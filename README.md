# QuantEdge
A modern C++ framework for implementing and testing quantitative trading strategies, including pairs trading, mean-reversion, and momentum strategies. It focuses on efficiency, scalability, and modularity, making it suitable for quantitative finance projects.

# Deal Creation Tool

A Python-based tool for creating and managing financial deals with facilities and outstandings in Loan IQ.

## Components

### deal_creator.py

The main script that handles deal creation and management through a series of structured steps.

#### Key Features
- Creates deals with configurable parameters (name, alias, dates, amounts)
- Manages facility creation and validation
- Handles outstanding creation, validation, and release
- Processes facility and deal closure
- Uses JSON configuration for deal parameters

#### Classes
- `DealConfig`: Validates and stores deal configuration
- `DealParams`: Manages deal-specific parameters
- `FacilityParams`: Handles facility-related parameters
- `OutstandingParams`: Manages outstanding-specific parameters
- `DealCreator`: Main class orchestrating the deal creation process

#### Command Line Usage
```bash
deal_creator create-deal [OPTIONS]

Options:
--config PATH         JSON config file path (default: cfg/deal_creation/deal_creation.json)
--template-path TEXT  Template directory for XML queries (default: cre-deal)
```

### query_executor.py

Handles XML query execution and response parsing for LoanIQ interactions.

#### Key Features
- Executes XML queries against LoanIQ
- Parses colon-separated responses
- Extracts IDs from nested response structures
- Manages both REST and XQuery client connections

#### Classes
- `QueryResponse`: Models query response structure
- `QueryExecutor`: Handles query execution and response processing

## Configuration

### JSON Config Structure
```json
{
  "cre-deal": {
    "deal_name": "DEAL_NAME",
    "deal_alias": "DEAL_ALIAS",
    "agreement_date": "YYYY-MM-DD",
    "principal_amount": 1000000,
    "rate": 7.75,
    "tenor": 10
  }
}
```

### Required XML Templates
1. create_deal_CRE.xml
2. create_facility_CRE.xml
3. validate_facility_CRE.xml
4. create_outstanding_CRE.xml
5. validate_outstanding_CRE.xml
6. release_outstanding_CRE.xml
7. close_facility_CRE.xml
8. close_deal_CRE.xml

## Dependencies
- pathlib
- datetime
- dateutil
- pydantic
- click
- returns
- ldds.liq_client
- cfg_reader

