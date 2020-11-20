#include "../include/types.h"
#include "../include/pokemon.h"

struct windows{
	void *unk0;
	u8 unk4;
	u8 unk5;
	u8 unk6;
	u8 unk7;
	u8 unk8;
	u8 unk9;
	u16 unkA;
	void *unkC;
};

struct unk_struct{
	void *unk0;
	void *unk4;
	void *unk8;
	u32 unkC;
	u8 unk10;
	u8 unk11;
	/*and more*/
	/*........*/
};

struct state{
	void * unk0;
	struct windows win[36];
	struct windows *add_win;
	u32	win_max;
	struct unk_struct *data;
	void * monname;
	void * nickname;
	void * unk_pokedata;
	u16 mon_id;
	u16 item;
	u8 type1;
	u8 type2;
	u16 unk_pokedata2;
	u32 id;
	u32 unk_pokedata3;
	u32 unk_pokedata4;
	u32 unk_pokedata5;
	u16 hp;
	u16 max_hp;
	u16 atk;
	u16 def;
	u16 spa;
	u16 spd;
	u16 spe;
	/*and more*/
	/*........*/
};

#define MON_DATA_HP_MAX 164
#define MON_DATA_HP_EV 13
#define MON_DATA_HP_IV 70
#define MON_DATA_HP 163

static void PokeDataSet2(struct state *poke,u8 mode)
{
	void *pokemon = PokeStatusGet(poke);
	int start = MON_DATA_HP_MAX;
	if(mode == 1)
		start = MON_DATA_HP_EV;
	else if(mode == 2)
		start = MON_DATA_HP_IV;
	if(poke->data->unk11 == 2)
	{
		void *pokemon2 = PokemonAlloc(19);
		PokeReplace(pokemon, pokemon2);
		if(!mode)
			poke->hp =(u16)GetMonData(pokemon2, MON_DATA_HP, NULL);
		else
			poke->hp = (u16)GetMonData(pokemon2, start, NULL);
		poke->atk = (u16)GetMonData(pokemon2, start+1, NULL);
		poke->def = (u16)GetMonData(pokemon2, start+2, NULL);
		poke->spa = (u16)GetMonData(pokemon2, start+4, NULL);
		poke->spd = (u16)GetMonData(pokemon2, start+5, NULL);
		poke->spe = (u16)GetMonData(pokemon2, start+3, NULL);
		FreeSystem(pokemon2);
	}
	else
	{
		if(!mode)
			poke->hp = (u16)GetMonData(pokemon, MON_DATA_HP, NULL);
		else
			poke->hp = (u16)GetMonData(pokemon, start, NULL);
		poke->atk = (u16)GetMonData(pokemon, start+1, NULL);
		poke->def = (u16)GetMonData(pokemon, start+2, NULL);
		poke->spa = (u16)GetMonData(pokemon, start+4, NULL);
		poke->spd = (u16)GetMonData(pokemon, start+5, NULL);
		poke->spe = (u16)GetMonData(pokemon, start+3, NULL);
	}
}

#define color(a1, a2, a3) (u32)((((a1)&0xff) << 16)|(((a2)&0xff) << 8)|(((a3)&0xff) << 0))
#define black_color	(color( 1, 2, 0 ))

void ChangeState(struct state *poke ,u8 mode)
{
	FillWindowPixelBuffer( &poke->add_win[0], 0);
	FillWindowPixelBuffer( &poke->add_win[1], 0);
	FillWindowPixelBuffer( &poke->add_win[2], 0);
	FillWindowPixelBuffer( &poke->add_win[3], 0);
	FillWindowPixelBuffer( &poke->add_win[4], 0);
	FillWindowPixelBuffer( &poke->add_win[5], 0);
	PokeDataSet2(poke,mode);
	ConvertNumToString(poke, 119, poke->hp, 3, 1);
	StringPut(poke, &poke->add_win[0], black_color, 2);
	ConvertNumToString( poke, 120, poke->atk, 3, 0);
	StringPut(poke, &poke->add_win[1], black_color, 1);
	ConvertNumToString( poke, 121, poke->def, 3, 0 );
	StringPut(poke, &poke->add_win[2], black_color, 1);
	ConvertNumToString( poke, 122, poke->spa, 3, 0 );
	StringPut(poke, &poke->add_win[3], black_color, 1);
	ConvertNumToString( poke, 123, poke->spd, 3, 0 );
	StringPut(poke, &poke->add_win[4], black_color, 1);
	ConvertNumToString( poke, 124, poke->spe, 3, 0 );
	StringPut(poke, &poke->add_win[5], black_color, 1);

	PutWindows( &poke->add_win[0] );
	PutWindows( &poke->add_win[1] );
	PutWindows( &poke->add_win[2] );
	PutWindows( &poke->add_win[3] );
	PutWindows( &poke->add_win[4] );
	PutWindows( &poke->add_win[5] );
	PokeDataSet2(poke,0);//if you don't want recovery when you change page ,please delete this line
}