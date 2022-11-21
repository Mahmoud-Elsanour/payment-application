#include "app.h"

ST_cardData_t card1; //the user's card
ST_cardData_t* pCard = &card1; //pointer to the user's card

ST_terminalData_t terminal1;//the terminal data
ST_terminalData_t* pTerm = &terminal1;//pointer to the terminal data

ST_transaction_t transaction1;//a temporary variable to save the card and terminal data
ST_transaction_t* pTran = &transaction1;//pointer to the temporary variable

void appStart(void)
{
	//card
	NAME:
	if (getCardHolderName(pCard) == WRONG_NAME)
	{
		printf("Enter a vaild name please\n");
		goto NAME;
	}
	DATE:
	if (getCardExpiryDate(pCard) == WRONG_EXP_DATE)
	{
		printf("Enter the exp date in the following format please: 'mm/yy'\n");
		goto DATE;
	}
	PAN:
	if (getCardPAN(pCard) == WRONG_PAN)
	{
		printf("Please enter a vaild pan\n");
		goto PAN;
	}

	
	//terminal
	getTransactionDate(pTerm);
	if (isCardExpired(card1) == EXPIRED_CARD)
	{
		printf("EXPIRED CARD\n");
		return;
	}

	AMOUNT:
	if (getTransactionAmount(pTerm) == INVALID_AMOUNT)
	{
		printf("Please Enter a valid amount (not 0 or a negative number)\n");
		goto AMOUNT;
	}

	float maxAmount = 10000.0;
	if (setMaxAmount(pTerm, maxAmount) == INVALID_MAX_AMOUNT)
	{
		printf("Error in the allowed maxmium amount\n");
		return;
	}
	
	if (isBelowMaxAmount(pTerm) == EXCEED_MAX_AMOUNT)
	{
		printf("Amount is exceeding the limit\n");
		return;
	}

	//transf
	transaction1.cardHolderData = card1;
	transaction1.terminalData = terminal1;


	//server
	recieveTransactionData(pTran);
	saveTransaction(pTran);
}