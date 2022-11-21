#ifndef server_h
#define server_h
#include "../Terminal/terminal.h"

typedef unsigned int uint32_t;


typedef enum accountState
{
	RUNNING,
	BLOCKED
}EN_accountState_t;

typedef enum transState
{
	APPROVED, INTERNAL_SERVER_ERROR, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD
}EN_transState_t;

typedef struct transaction
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef enum serverError
{
	SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE
}EN_serverError_t;

typedef struct accountsDB
{
	float balance;
	char primaryAccountNumber[20];
	EN_accountState_t accountState;
}ST_accountsDB_t;

EN_transState_t recieveTransactionData(ST_transaction_t* transData);
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isBlockedAccount();
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData);

#endif // !server_h