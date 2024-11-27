# QuantEdge
A modern C++ framework for implementing and testing quantitative trading strategies, including pairs trading, mean-reversion, and momentum strategies. It focuses on efficiency, scalability, and modularity, making it suitable for quantitative finance projects.

from pathlib import Path
from datetime import datetime
from dateutil.relativedelta import relativedelta
from typing import Any, Optional, Dict
import json, pwd, os, random, logging, click, sys
from ldds.liq_client.cfg import LoanIqInquiryApiConfig, LoanIqRestApiConfig
from ldds.liq_client.xquery import LoanIqInquiryClient
from ldds.liq_client.rest import LIQRestClient
from cfg_reader import config_reader
from returns.pipeline import is_successful
from pydantic import validate_call, BaseModel, Field, validator

from .query_executor import QueryExecutor

DEFAULT_CONFIG = Path(__file__).parent.parent / "cfg" / "deal_creation" / "deal_creation.json"

logging.basicConfig(
    format="%(asctime)s [%(name)s] %(levelname)s: %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
    level=logging.INFO
)

logger = logging.getLogger(__name__)

class DealConfig(BaseModel):
    deal_name: str = Field(..., min_length=1)
    deal_alias: str = Field(..., min_length=1)
    agreement_date: datetime
    principal_amount: int = Field(..., gt=0)
    rate: float = Field(..., gt=0, le=100)
    tenor: int = Field(..., gt=0)

    @validator("agreement_date", pre=True)
    def parse_date(cls, value):
        try:
            return datetime.strptime(value, "%Y-%m-%d")
        except ValueError as e:
            raise ValueError(
                f"Invalid date format for 'agreement_date': {value}. Expected format: YYYY-MM-DD."
            ) from e

    def __str__(self):
        return (
            f"deal_name='{self.deal_name}' deal_alias='{self.deal_alias}' "
            f"agreement_date='{self.agreement_date.strftime('%Y-%m-%d')}' "
            f"rate='{self.rate}' tenor='{self.tenor}'"
        )

class DealParams(BaseModel):
    deal_id: str
    facility_id: Optional[str] = None
    transaction_id: Optional[str] = None
    user_id: str
    maturity_date: str

class FacilityParams(BaseModel):
    deal_id: str
    facility_name: str
    maturity_date: str
    closing_commitment: int
    proposed_commit_amount: int
    spread: float

class OutstandingParams(BaseModel):
    effective_date: str
    maturity_date: str
    facility_control_number: str
    requested_amount: float
    amount: float

class DealCreator:
    def __init__(self, template_path: str, config: DealConfig):
        self.template_path = Path(template_path)
        self.config = config
        self.deal_params = DealParams(
            deal_id="",
            user_id=pwd.getpwuid(os.geteuid()).pw_name.ljust(6),
            maturity_date=self._calculate_maturity_date()
        )
        self.query_executor = QueryExecutor()
        self.facility_params = None
        self.outstanding_params = None

    @classmethod
    def from_config_file(cls, template_path: str, config_path: str):
        config_data = parse_json_cfg(config_path)
        if template_path not in config_data:
            raise ValueError(f"Template '{template_path}' not found in config file")
        deal_config = DealConfig(**config_data[template_path])
        return cls(template_path=template_path, config=deal_config)

    def _calculate_maturity_date(self) -> str:
        maturity_date = self.config.agreement_date + relativedelta(years=self.config.tenor)
        return maturity_date.strftime("%Y-%m-%d")

    def create_deal(self) -> Optional[str]:
        template_params = {
            "deal_name": self.config.deal_name,
            "deal_alias": self.config.deal_alias,
            "agreement_date": self.config.agreement_date.strftime("%Y-%m-%d"),
            "user_id": self.deal_params.user_id,
        }
        deal_id = self.query_executor.execute_and_check(
            str(Path(self.template_path) / "create_deal_CRE.xml"),
            template_params,
            "@dealId"
        )
        if deal_id:
            self.deal_params.deal_id = deal_id
            return deal_id
        return None

    def create_facility(self) -> Optional[str]:
        self.facility_params = FacilityParams(
            deal_id=self.deal_params.deal_id,
            facility_name=f"{self.config.deal_name}_FAC",
            maturity_date=self.deal_params.maturity_date,
            closing_commitment=self.config.principal_amount,
            proposed_commit_amount=self.config.principal_amount,
            spread=self.config.rate
        )
        
        template_params = {
            "deal_id": self.deal_params.deal_id,
            "facility_name": self.facility_params.facility_name,
            "maturity_date": self.facility_params.maturity_date,
            "closing_commitment": self.facility_params.closing_commitment,
            "proposed_commit_amount": self.facility_params.proposed_commit_amount,
            "spread": self.facility_params.spread,
        }
        
        facility_id = self.query_executor.execute_and_check(
            str(Path(self.template_path) / "create_facility_CRE.xml"),
            template_params,
            "@facilityId"
        )
        if facility_id:
            self.deal_params.facility_id = facility_id
            return facility_id
        return None

    def validate_facility(self) -> bool:
        template_params = {"facility_id": self.deal_params.facility_id}
        return self.query_executor.execute_and_check(
            str(Path(self.template_path) / "validate_facility_CRE.xml"),
            template_params
        ) is not None

    def create_outstanding(self) -> Optional[str]:
        self.outstanding_params = OutstandingParams(
            effective_date=self.config.agreement_date.strftime("%Y-%m-%d"),
            maturity_date=self.deal_params.maturity_date,
            facility_control_number=self.deal_params.facility_id,
            requested_amount=float(self.config.principal_amount),
            amount=float(self.config.principal_amount)
        )
        
        template_params = {
            "effective_date": self.outstanding_params.effective_date,
            "maturity_date": self.outstanding_params.maturity_date,
            "facility_control_number": self.outstanding_params.facility_control_number,
            "requested_amount": self.outstanding_params.requested_amount,
            "amount": self.outstanding_params.amount
        }
        
        outstanding_id = self.query_executor.execute_and_check(
            str(Path(self.template_path) / "create_outstanding_CRE.xml"),
            template_params,
            "@outstandingId"
        )
        if outstanding_id:
            self.deal_params.transaction_id = outstanding_id
            return outstanding_id
        return None

    def validate_outstanding(self) -> bool:
        template_params = {"transaction_id": self.deal_params.transaction_id}
        return self.query_executor.execute_and_check(
            str(Path(self.template_path) / "validate_outstanding_CRE.xml"),
            template_params
        ) is not None

    def release_outstanding(self) -> bool:
        template_params = {"transaction_id": self.deal_params.transaction_id}
        return self.query_executor.execute_and_check(
            str(Path(self.template_path) / "release_outstanding_CRE.xml"),
            template_params
        ) is not None

    def close_facility(self) -> bool:
        template_params = {"facility_id": self.deal_params.facility_id}
        return self.query_executor.execute_and_check(
            str(Path(self.template_path) / "close_facility_CRE.xml"),
            template_params
        ) is not None

    def close_deal(self) -> bool:
        template_params = {"deal_id": self.deal_params.deal_id}
        return self.query_executor.execute_and_check(
            str(Path(self.template_path) / "close_deal_CRE.xml"),
            template_params
        ) is not None

    def process_deal(self) -> bool:
        try:
            logger.info("Creating deal...")
            if not self.create_deal():
                return False
            
            logger.info("Creating facility...")
            if not self.create_facility():
                return False
                
            logger.info("Validating facility...")
            if not self.validate_facility():
                return False
                
            logger.info("Creating outstanding...")
            if not self.create_outstanding():
                return False
                
            logger.info("Validating outstanding...")
            if not self.validate_outstanding():
                return False
            
            logger.info("Releasing outstanding...")
            if not self.release_outstanding():
                return False
            
            logger.info("Closing facility...")
            if not self.close_facility():
                return False
            
            logger.info("Closing deal...")
            if not self.close_deal():
                return False
                
            logger.info("Deal processing completed successfully")
            return True
        except Exception as e:
            logger.error(f"Failed to process deal: {e}")
            return False

def parse_json_cfg(config: str) -> dict:
    try:
        logger.info(f"Parsing file: {config}")
        with open(config, "r") as f:
            content = f.read()
            logger.debug(f"Raw JSON content:\n{content}")
            return json.loads(content)
    except json.JSONDecodeError as e:
        logger.error(f"Failed to parse JSON file: {e}")
        raise ValueError(f"Failed to parse JSON file: {e}")
    except Exception as e:
        logger.error(f"Failed to load config file: {e}")
        raise ValueError(f"Failed to load config file: {e}")

@click.group()
def main():
    pass

@main.command(help="Create Deals with a single facility and outstanding")
@click.option("--config", type=click.Path(exists=True), default=str(DEFAULT_CONFIG),
              help="Path to JSON Config file")
@click.option("--template-path", type=str, required=False, default="cre-deal",
              help="Section in the TOML file to use for the deal creation parameters")
@validate_call
def create_deal(config: str, template_path: str) -> None:
    try:
        creator = DealCreator.from_config_file(template_path=template_path, config_path=config)
        if not creator.process_deal():
            logger.error("Failed to create deal")
            sys.exit(1)
    except Exception as e:
        logger.error(f"Unexpected error during deal creation: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
