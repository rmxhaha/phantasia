#include <iostream>
#include <map>

using namespace std;

struct data_monster_struct {
	int hp_awal;
	int sp_awal;
};

struct monster_di_stage_struct {
	int hp;
	int sp;
};

int main(){
	map< string, data_monster_struct > data_monster;
	map< string, monster_di_stage_struct > monster_di_stage;

	typedef map< string, monster_di_stage_struct>::iterator my_iterator;

	//isi dulu data_monster nya
	data_monster["ouroboros"].hp_awal = 10;
	data_monster["ouroboros"].sp_awal = 5;
	data_monster["poring"].hp_awal = 5;
	data_monster["poring"].sp_awal = 5;

	//ini data monster yg kta pake di battle
	//gak apa2 di otak atik
	//setelah battle di hapus
	monster_di_stage["ouroboros_1"].hp = data_monster["ouroboros"].hp_awal;
	monster_di_stage["ouroboros_1"].sp = data_monster["ouroboros"].sp_awal;
	monster_di_stage["ouroboros_2"].hp = data_monster["ouroboros"].hp_awal;
	monster_di_stage["ouroboros_2"].sp = data_monster["ouroboros"].sp_awal;
	monster_di_stage["poring_1"].hp = data_monster["poring"].hp_awal;
	monster_di_stage["poring_1"].sp = data_monster["poring"].sp_awal;


	// === START BATTLE ===
	int damage_hero = 1;
	string nama_monster;


	my_iterator itr;

	while( true ){
		cout << "Attack Target :";
		cin >> nama_monster;

//tanda 1
		itr = monster_di_stage.find( nama_monster );

		if( itr != monster_di_stage.end() ){ //index not found
			itr -> second.hp -= damage_hero;
		}

//tanda 2

	/*
		sebenernya code anatra tanda 1 sama tanda 2
		bsa di buat simple kyk di bawah ini

		monster_di_stage[ nama_monster ].hp -= damage_hero;

		tapi untuk cari aman
		siapa tau lu input nama monster yang tidak ada


		itr = monster_di_stage.find( nama_monster );

		arti nya mencari di array "monster_di_stage" yang indexnya "nama_monster"
		sama dengan monster_di_stage[nama_monster]

		tapi kali ini pake iterator
		kalo lu gak ngerti iterator
		hmmm....

		anggep aja kayak gini
		iterator itu pointer buat looping
		iterator itu bsa di pakai sebagai pointer biasa juga

		monster_di_stage.find( nama_monster );

		return dari function di atas itu pointer ke data yang di cari
		yang di masukan ke variable "itr"

		kalo gak ketemu return monster_di_stage.end()
		ini iterator dari end of array

		kalo iterator buat looping itu
		jadi iterator itu bsa di increment (++)

		itr++;
		tapi jangan keterusan nanti lu ke point ke data yg tidak seharusnya ada di array

		kenapa gak pake index
		karena indexnya gak berurut di map
		kalo vector emang bsa pake kyk bsa looping pake index
		karena indexnya beurut 0 sampai X

		kalo map harus pake iterator
	*/


		for( itr = monster_di_stage.begin(); itr != monster_di_stage.end(); itr++ ){
			cout << itr -> first << " : ";
			cout << "( " << (itr -> second).hp << " " << (itr -> second).sp << " )\n";
		}
	}

}
