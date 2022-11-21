#pragma warning(disable: 4996)
#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	//getting the card holder's name
	printf("Please enter the card holder name: \n");
	gets(& cardData->cardHolderName);

	for (int i = 0; i < strlen(cardData->cardHolderName); i++)
	{
		if (cardData->cardHolderName[i] == ' ')
		{
			cardData->cardHolderName[i] = '_';
		}
	}

	//checks the name status
	if (strlen(cardData->cardHolderName) < 20 || strlen(cardData->cardHolderName) > 24)
	{
		return WRONG_NAME;
	}
	return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	//getting the exp date 
	printf("Please enter the exp date 'mm/yy' :  \n");
	gets(& cardData->cardExpirationDate);

	//checks the exp date status
	if (strlen(cardData->cardExpirationDate) != 5 || cardData->cardExpirationDate[2] != '/')
	{
		return WRONG_EXP_DATE;
	}
	return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	//getting the PAN 
	printf("Please enter the PAN: \n");
	gets(&cardData->primaryAccountNumber);

	//checks the PAN status
	if (strlen(cardData->primaryAccountNumber) < 16 || strlen(cardData->primaryAccountNumber) > 19)
	{
		return WRONG_PAN;
	}
	return OK;
}
