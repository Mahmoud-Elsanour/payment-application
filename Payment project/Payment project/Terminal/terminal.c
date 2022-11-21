#pragma warning(disable: 4996)
#include "terminal.h"
#include <time.h>


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	//getting the transaction date from the pc
	time_t t = time(NULL);
	struct tm* tm = localtime(&t);
	strftime(termData->transactionDate, sizeof termData->transactionDate, "%d/%m/%Y", tm);
}

EN_terminalError_t isCardExpired(ST_cardData_t cardData)
{
	//converting the exp month to an integer
	char x0 = cardData.cardExpirationDate[0];
	char x1 = cardData.cardExpirationDate[1];
	int m = (x1 - '0' + (x0 - '0') * 10);

	//converting the exp year to an integer
	char x3 = cardData.cardExpirationDate[3];
	char x4 = cardData.cardExpirationDate[4];
	int y = (x4 - '0' + (x3 - '0') * 10);

	//getting the pc time
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	//checking the time 
	if (y == tm.tm_year - 100 && m < (tm.tm_mon + 1))
	{
		return EXPIRED_CARD;
	}
	else if (y < tm.tm_year - 100)
	{
		return EXPIRED_CARD;

	}
	else
		return OKTerminal;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	//getting the transaction amount
	printf("Please enter the transaction amount: \n");
	scanf("%f", &termData->transAmount);

	//checks the transaction amount status
	if (termData->transAmount <= 0) {
		return INVALID_AMOUNT;
	}
	return OKTerminal;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	//checking if the wanted amount above 10k or not
	if (termData->transAmount > termData->maxTransAmount)
		return EXCEED_MAX_AMOUNT;
	return OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	if (maxAmount <= 0)
	{
		return INVALID_MAX_AMOUNT;
	}
	termData->maxTransAmount = maxAmount;
	return OK;
}
