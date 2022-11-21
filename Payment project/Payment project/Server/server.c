#pragma warning(disable: 4996)
#include "server.h"

uint8_t tsn = 0; //(transactionSequenceNumber) a variable used to change the saving place in the transaction data base
uint8_t* p3 = &tsn; //a pointer to the tsn to change its value
int i; //a variable to be used in linear searching with the for loop
int* p = &i; // a pointer to the searching variable to chasnge its value
EN_transState_t trans_state; //a variable to save the transfer statue 
EN_transState_t* P = &trans_state;//a pointer to the transfer variable to change its value
int old_balance; //variable to saves the old balance
int* balancePointer = &old_balance;

// the data base
ST_accountsDB_t serverDB[255] = { {2000 , "1234567890123456789" , RUNNING,},
								{4000 , "123456789012345678" , BLOCKED,}, 
								{6000 , "12345678901234567" , RUNNING,}, 
								{1500 , "1234567890123456" , RUNNING,}, 
								{1000 , "1234567890123455" , BLOCKED} };
ST_transaction_t transactionDB[255] = { 0 };

EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	int account_found = 0; //variable to save decide whether the account is found or not
	//linear search loop
	for (*p = 0; *p < 255; (*p)++)
	{
		if (!strcmp(cardData->primaryAccountNumber, serverDB[*p].primaryAccountNumber))
		{
			account_found = 1;
			break;
		}
	}
	//checking the account status
	if (account_found == 1)
		return SERVER_OK;
	else
	{
		printf("\nThis card is fake\n");
		return ACCOUNT_NOT_FOUND;
	}
}

EN_serverError_t isBlockedAccount()
{
	if (serverDB[*p].accountState == BLOCKED)
	{
		printf("\nAccess to this account is blocked\n");
		return DECLINED_STOLEN_CARD;
	}
	return SERVER_OK;
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	//checking if the amount is avilble
	if (termData->transAmount > serverDB[*p].balance)
	{
		printf("\nThis amount of money is not available\n");
		return LOW_BALANCE;
	}
	return SERVER_OK;
}


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	//recieving the acount statue
	*P = isValidAccount(&transData->cardHolderData);
	if (*P == 0)
		*P = isBlockedAccount();
	else
		return FRAUD_CARD;
	if (*P == 0)
		*P = isAmountAvailable(&transData->terminalData);
	else
		return DECLINED_STOLEN_CARD;
	//saving the new account balance if the transaction is OK
	if (*P == 0)
	{
		*balancePointer = serverDB[*p].balance;
		serverDB[*p].balance -= transData->terminalData.transAmount;
		if (old_balance != serverDB[*p].balance + transData->terminalData.transAmount)
		{
			(* P) == INTERNAL_SERVER_ERROR;
			return INTERNAL_SERVER_ERROR;
		}
	}
	else
		return DECLINED_INSUFFECIENT_FUND;
	return APPROVED;
}

EN_serverError_t getTransaction(uint32_t transactionSequenceNumber, ST_transaction_t* transData)
{
	//linear search loop
	for (int j = 0; j < 255; j++)
	{
		//checking if transaction sequence number is found
		if (transactionDB[j].transactionSequenceNumber == transactionSequenceNumber)
		{
			*transData = transactionDB[j];
			return SERVER_OK;
		}
	}
	return TRANSACTION_NOT_FOUND;
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	//updating the sequence number and the state
	transactionDB[*p3].transactionSequenceNumber = *p3 + 1;
	//checking that the tsn is not above data base limit
	if (transactionDB[*p3].transactionSequenceNumber > 255)
	{
		serverDB[*p].balance = old_balance;
		printf("saving failed!!\n");
		return SAVING_FAILED;
	}
	transactionDB[*p3].transState = trans_state;
	//saving card data
	transactionDB[*p3].cardHolderData = transData->cardHolderData;
	//saving terminal data
	transactionDB[*p3].terminalData = transData->terminalData;
	//Temporary variable to store the saved data
	ST_transaction_t tempTransData ;
	//setting the temporary variable value to the saved data value
	getTransaction(transactionDB[*p3].transactionSequenceNumber, &tempTransData);
	//comparing the temporary variable and the entered data
	if ( strcmp(tempTransData.cardHolderData.cardExpirationDate , transData->cardHolderData.cardExpirationDate) || strcmp(tempTransData.cardHolderData.cardHolderName , transData->cardHolderData.cardHolderName) || strcmp(tempTransData.cardHolderData.primaryAccountNumber , transData->cardHolderData.primaryAccountNumber) || tempTransData.terminalData.maxTransAmount != transData->terminalData.maxTransAmount || strcmp(tempTransData.terminalData.transactionDate , transData->terminalData.transactionDate)  || tempTransData.terminalData.transAmount != transData->terminalData.transAmount  || *P == INTERNAL_SERVER_ERROR)
	{
		//making sure the balance is still the same
		serverDB[*p].balance = old_balance;
		printf("saving failed!!\n");
		return SAVING_FAILED;
	}
	if (transactionDB[*p3].transState == 0)
		printf("\nSccessful transaction\n");
	else
		printf("\nUnsccessful transaction\n");
	printf("Transaction sequence number:%d\nBalance:%f\nName:%s\nDate:%s\n",transactionDB[*p3].transactionSequenceNumber, serverDB[*p].balance, transactionDB[*p3].cardHolderData.cardHolderName, transactionDB[*p3].terminalData.transactionDate);
	//updating tsn value so it points to the next place in the transactionDB array
	*p3 = transactionDB[*p3].transactionSequenceNumber;
	return SERVER_OK;
}