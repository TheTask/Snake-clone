#include <conio.h>
#include <stdio.h>
#include <ctime>
#include <vector>
#include <windows.h>

using namespace std;

#define vyskaPlochy 20
#define sirkaPlochy 50

struct ClanokHada
{
	ClanokHada(){}
	ClanokHada( short x1,short y1 )
	{
		x = x1;
		y = y1;
	}
	short x;
	short y;
};

unsigned short dlzkaHada = 3;
char poslednySmer = 'd';
vector< ClanokHada > had;
inline bool operator==( const ClanokHada &lavaStrana,const ClanokHada &pravaStrana );

unsigned short xPotrava;
unsigned short yPotrava;

char plocha[ vyskaPlochy * sirkaPlochy ];

const void ZobrazPlochu();
void NormalizujPlochu();
const void InicializujHada();
const void InicializujPotravu();
void Pohyb();
void ZmazKoniecHada();
bool ZozralPotravu();

void VycistiKonzolu();

bool Vyhral();
bool Prehral();

int main()
{
	srand( (unsigned int)time( NULL ) );

	NormalizujPlochu();
	InicializujHada();
	InicializujPotravu();

	for( ;; )
	{
		std::clock_t start;
		double duration = 0.0;
		start = clock();

		while( duration < 0.1 )
		{
			duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
		}

		if( _kbhit() )
		{
			poslednySmer = _getch();
		}
		
		if( ZozralPotravu() ) 
		{
			dlzkaHada++;
			ClanokHada clanok( xPotrava,yPotrava );
			had.push_back( clanok );
			InicializujPotravu();
		}

		VycistiKonzolu();
		ZobrazPlochu();
		Pohyb();

		if( Vyhral() )
		{
			printf( "Gratulujem, vyhral si!" );
		}
		if( Prehral() )
		{
			printf( "Prehral si!" );
			break;
		}
	}
}
const void ZobrazPlochu() 
{
	for( unsigned short i = 0; i < sirkaPlochy + 2; i++ ) printf( "%c",'/' ); printf( "\n" ); //horna hranica

	for( unsigned short y = 0; y < vyskaPlochy; y++ )
	{
		printf( "%c",'/' );
		for( unsigned short x = 0; x < sirkaPlochy; x++ )
		{
			printf( "%c",plocha[ y * sirkaPlochy + x ] );
		}
		printf( "%c\n",'/' );
	}
	
	for( unsigned short i = 0; i < sirkaPlochy + 2; i++ ) printf( "%c",'/' ); printf( "\n" ); //spodna hranica	
	printf( "Dlzka hada: %d\n",dlzkaHada );
}
void NormalizujPlochu()
{
	for( unsigned short x = 0; x < sirkaPlochy; x++ )
	{
		for( unsigned short y = 0; y < vyskaPlochy; y++ )
		{
			 plocha[ y * sirkaPlochy + x ] = ' ';
		}
	}
}
const void InicializujHada()
{
	for( unsigned short i = 0; i < dlzkaHada; i++ )
	{
		ClanokHada clanok;
		clanok.x = i;
		clanok.y = 0;
		had.push_back( clanok );
		plocha[ i ] = 'O';
	}
}
const void InicializujPotravu()
{
	xPotrava = rand() % sirkaPlochy;
	yPotrava = rand() % vyskaPlochy;

	while( plocha[ yPotrava * sirkaPlochy + xPotrava ] != ' ' )
	{
		xPotrava = rand() % sirkaPlochy;
		yPotrava = rand() % vyskaPlochy;
	}

	plocha[ yPotrava * sirkaPlochy + xPotrava ] = 'X'; 
}
void Pohyb()
{
	if( poslednySmer == 'd' )
	{
		ClanokHada clanok = had.at( had.size() - 1 ); //posledny clanok je vlastne prvy
		clanok.x++;
		had.push_back( clanok );
		plocha[ clanok.y * sirkaPlochy + clanok.x ] = 'O';

		ZmazKoniecHada();
	}
	else if( poslednySmer == 's' )
	{
		ClanokHada clanok = had.at( had.size() - 1 ); 
		clanok.y++;
		had.push_back( clanok );
		plocha[ clanok.y * sirkaPlochy + clanok.x ] = 'O';

		ZmazKoniecHada();
	}
	else if( poslednySmer == 'a' )
	{
		ClanokHada clanok = had.at( had.size() - 1 ); 
		clanok.x--;
		had.push_back( clanok );
		plocha[ clanok.y * sirkaPlochy + clanok.x ] = 'O';

		ZmazKoniecHada();
	}
	else if( poslednySmer == 'w' )
	{
		ClanokHada clanok = had.at( had.size() - 1 ); 
		clanok.y--;
		had.push_back( clanok );
		plocha[ clanok.y * sirkaPlochy + clanok.x ] = 'O';

		ZmazKoniecHada();
	}
	else
	{}

}
void ZmazKoniecHada()
{
	ClanokHada temp = had[ 0 ]; //prvy je vlastne posledny
	plocha[ temp.y * sirkaPlochy + temp.x ] = ' ';
	had.erase( had.begin() );
}
bool ZozralPotravu()
{
	ClanokHada clanok = had.at( had.size() - 1 );
	ClanokHada potrava;
	potrava.x = xPotrava;
	potrava.y = yPotrava;
	return clanok == potrava;
}
void VycistiKonzolu()
{
	HANDLE hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	COORD coord = { 0,0 };
	DWORD count;
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	if( GetConsoleScreenBufferInfo( hStdOut,&csbi ) )
	{
		FillConsoleOutputCharacter( hStdOut,(TCHAR)32,csbi.dwSize.X * csbi.dwSize.Y,coord,&count );
		FillConsoleOutputAttribute( hStdOut,csbi.wAttributes,csbi.dwSize.X - 1* csbi.dwSize.Y - 1,coord,&count );
		SetConsoleCursorPosition( hStdOut,coord );
	}
	return;
}
bool Vyhral()
{
	bool medzera = false;
	for( unsigned short x = 0; x < sirkaPlochy; x++ )
	{
		for( unsigned short y = 0; y < vyskaPlochy; y++ )
		{
			 if( plocha[ y * sirkaPlochy + x ] == ' ' )
			 {
				medzera = true;
				break;
			 }
		}
	}
	return !medzera;
}
bool Prehral()
{
	ClanokHada clanok = had.at( had.size() - 1 );

	if( clanok.x < 0 || 
		clanok.x > sirkaPlochy - 1 || 
		clanok.y < 0 ||
		clanok.y > vyskaPlochy - 1 ) return true;
	
	for( unsigned short i = 0; i < had.size() - 1; i++ )
	{
		ClanokHada temp = had[ i ];
		if( temp == clanok ) return true;
	}

	return false;
}
inline bool operator==( const ClanokHada &lavaStrana,const ClanokHada &pravaStrana )
{
	return lavaStrana.x == pravaStrana.x && lavaStrana.y == pravaStrana.y;
}