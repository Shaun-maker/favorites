#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//prototype
int clear(void);
bool search_or_add();
bool check_exit(char * var_fav);
char * get_fav();
char * get_tag();
int list_tag(FILE * file);
int add_comment(FILE *file);

// global variable
char mexit[5] = "exit";
char mstop[5] = "stop";
char var_fav[3000];

int cmp = 1;

int main(void)
{
	FILE *file = fopen(".favorites.db", "a");
	if (file == NULL)
	{
		printf("Impossible to open the favorites file.\n");
		return 1;
	}

	search_or_add();

	char *var_fav = get_fav();

	fprintf(file, "%s|END|", var_fav);
	
	printf("Please enter tags one by one, enter 'stop' when you finish: \n");

	list_tag(file);

	add_comment(file);
			
	fclose(file);

	free(var_fav);

	exit(0);
}

//function to clear buffer
int clear (void)
{
	int ch;
	while ((ch = getchar()) != '\n' && ch != EOF);
	return ch;
}

bool search_or_add()
{
	bool check_correct_input = false;
	char user_input[100];

	printf("Do you want to add note, or search for an existing note ?\n");

	do
	{
		bool check_alpha = true;

		fgets(user_input, 100, stdin);

		int input_len = strlen(user_input);

		user_input[input_len - 1] = '\0';

		if (input_len >= 10)
		{
			printf("Too much character. Write 'search' or 's', 'add' or 'a'.\n");
			clear();
		}
		else
		{
			for (int i = 0; i < input_len - 1; i++)
			{
				if (!(isalpha(user_input[i])))
					check_alpha = false;

				if (isupper(user_input[i]))
					user_input[i] = tolower(user_input[i]);
			}
			if (check_alpha == false)
				printf("Vous avez tapé autre chose que des lettres\n");
			else
				printf("Vous avez bien tapé que des lettres\n");
		}
	}
	while (check_correct_input == false);
}

bool check_exit(char *var_fav)
{
	int value_cmp_exit = strcmp(mexit, var_fav);

	if (value_cmp_exit == 0)
	       return true;
	else
		return false;	
}


char* get_fav()
{
	char tmp[10000];
	printf("Please past or write your note: \n");
	fgets(tmp, 10000, stdin);

	int len = strlen(tmp) + 1;

	if (len > 9999)
	{
		printf("Fail: Your note cannot be longer than 10 000 char.\n");
		exit(1);
	}

	tmp[len - 2] = 0;

	char *var_fav = malloc(sizeof(char) * len);

	strcpy(var_fav, tmp);

	if (check_exit(var_fav) == true)
	{
		free(var_fav);
		exit(0);
	}

	return var_fav;
}

char * get_tag()
{
	char tmp[51];
	bool check_len = false;
	bool check_alpha = false;

	//check si le tag ne contient pas de numéro ou de symbole, uniquement des lettres. Convertir les majuscules en minuscules également.
	//TODO Il faudra soit check avec scanf si l'utilisateur à taper des espaces, soit check en utilisant fgets
	do
	{
		check_alpha = true;

		scanf("%s", tmp);

		int len = strlen(tmp);

		if (len <= 50)
			check_len = true;
		else
			printf("Fail: the maximum lenght of a tag is 50 char.\n");

		for (int i = 0; i < len; i++)
		{
			if (!(isalpha(tmp[i])))
				check_alpha = false;
			
			if (isupper(tmp[i]))
				tmp[i] = tolower(tmp[i]);
		}
		if (check_alpha == false)
			printf("Fail: Your tag must have only letters.\n");
	}
	while (check_alpha != true || check_len != true);
	
	int len = (strlen(tmp) + 1);

	char * var_tag = malloc(sizeof(len));

	strcpy(var_tag, tmp);

	return var_tag;
}

int list_tag(FILE * file)
{
	char mstop[5] = "stop";
	int i = 0;
	int cmp_stop = 1;

	//Check if the user put at least one tag. If not, prompt again.
	bool check_tag = false;

	char * list_tag[30];

	do
	{
		int cmp_same_word = 1;
		char * var_tag = get_tag();
		bool check_tag_exist = false;

		cmp_stop = strcmp(var_tag, mstop);

		if (cmp_stop == 0 && check_tag == false)
			printf("You have to put at least one tag, because it is the only way to find your note.\n");

		list_tag[i] = var_tag;

		if (i > 30)
		{
			printf("You can't have more than 30 tags.\n");
			return 1;
		}

		// Check if the tag was already type by the user. If not, add the tag to the file.
		for (int j = 0; j < i; j++)
		{
			cmp_same_word = strcmp(list_tag[i], list_tag[j]);
			if (cmp_same_word == 0 && cmp_stop != 0 && check_tag_exist != true)
			{
				printf("You already enter this tag.\n");
				check_tag_exist = true;
			}
		}

		if (check_tag_exist != true && cmp_stop != 0)
		{
			fprintf(file, "<%s>", list_tag[i]);
			check_tag = true;
		}

		i++;
	}
	while (cmp_stop != 0 || check_tag == false);

	for (int j = 0; j < i; j++)
		free(list_tag[j]);

	if (check_tag == true)
	{
		fprintf(file, "|END|");
		return 0;
	}
	else
		return 1;
}

int add_comment(FILE *file)
{
	char comment[3000];
	char answer_comment[20];
	char yes[4] = "yes";
	char no[3] = "no";
	char y[2] = "y";
	char n[2] = "n";
	bool check_comment = false;
	while ((getchar()) != '\n');

	printf("Do you want to add a comment ? (y/n)\n");
	do
	{
		fgets(answer_comment, 20, stdin);

		int answ_len = strlen(answer_comment);
		answer_comment[answ_len - 1] = 0;
		int cmpYes = strcmp(answer_comment, yes);
		int cmpY = strcmp(answer_comment, y);
		int cmpNo = strcmp(answer_comment, no);
		int cmpN = strcmp(answer_comment, n);
		int cmpExit = strcmp(answer_comment, mexit);

		if (answ_len > 18)
		{
			printf("Too much character\n");
			// clear input stream
			while ((getchar()) != '\n');

			check_comment = false;
		}
		else if (cmpYes == 0||cmpY == 0)
		{
			printf("Please enter your comment:\n");
	
			fgets(comment, 3000, stdin);
			//Ici, supprimer le dernier caractère de retour à la ligne de fgets
			int len = strlen(comment);
			comment[len - 1] = 0;
	
			fprintf(file, "'%s'", comment);
			fprintf(file, "|END|\n");
			check_comment = true;
			return 0;
		}
		else if (cmpNo == 0||cmpN == 0)
		{
			check_comment = true;
		       return 0;
		}
		else if (cmpExit == 0)
		{
			check_comment = true;
			return 0;
		}
		else
		{
			printf("Invalid choice\n");	
			check_comment = false;
		}
	}
	while (check_comment != true);
	return 1;
}

