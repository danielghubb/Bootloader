/* needs to stay the first line */
asm(".code16gcc\njmp $0, $main");

/* space for additional code */
/*Nimmt 8 Zeichen Tastaturinput verdeckt entgegen und gibt diese als char-array zurück.*/
void getPassword(char *password)
{
	int cnt;
	for (cnt = 0; cnt < 8; cnt++)
	{
		char letter;

		/*Lese Zeichen von der Tastatur und speichere es in letter*/
		asm volatile(
			"mov $0, %%eax;"
			"int $0x16;"
			"movb %%eax, %%edx;"
			: "=edx"(letter)::"eax");

		if (letter == 0x0D)
		{
			break;
		}

		/*Schreibe '*'*/
		asm(
			"mov $0xE2A, %%eax;"
			"mov $0xE, %%ah;"
			"int $0x10;" ::
				: "eax");

		password[cnt] = letter;
	}
}

/*Printed einen gegeben Text*/
void writeString(char *text)
{
	int cnt = 0;
	while (text[cnt] != '\0')
	{
		asm volatile(
			"movb %%edx, %%eax;"
			"mov $0xE, %%ah;"
			"int $0x10;" ::"edx"(text[cnt])
			: "eax");
		
		cnt++;
	}
	
}

/*Ueberschreibt den String mit Endungen.*/
void cleanPassword(char *password)
{
	int cnt = 0;
	while (password[cnt] != '\0')
	{
		password[cnt] = '\0';
		
		cnt++;
	}
}

/*Fügt eine neue Zeile ein und setzt den Cursor auf ganz links.*/
void jumpToNextLine()
{
	asm(
		"mov $0xE0A, %%eax; int $0x10;"
		"mov $0xE0D, %%eax; int $0x10;" ::
			: "eax");
}

/*Schreibe 'Reboot!' und starte neu.*/
void restart()
{
	writeString("Reboot!");
	jumpToNextLine();
	asm("int $0x19;");
}

void main(void)
{
	// greeting();
	writeString("Hello!");
	jumpToNextLine();

	/*
	Behilfsmäßiger boolean mit
	1 = true
	0 = false
	*/
	int boolean = 1;
	while (boolean == 1)
	{
		char password[9];
		cleanPassword(password);
		getPassword(password);
		if (password[0] == '\0')
		{
			boolean = 0;
		}
		else
		{
			jumpToNextLine();
			writeString(password);
			jumpToNextLine();
		}
	}
	restart();
}
