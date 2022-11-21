#pragma warning(disable: 4996)
#include "../Payment project/App/app.h"
char answer = 0;
int main()
{
		do_again:
		appStart();
		printf("do you want to make another transaction? (y = yes // n = no): \n");
		gets(& answer);
		ask_again:
		if (answer == 'y' || answer == 'Y')
			goto do_again;
		else if (answer == 'n' || answer == 'N')
			return;
		else
		{
			gets(&answer);
			goto ask_again;
		}
}