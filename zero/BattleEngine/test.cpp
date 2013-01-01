#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <math.h>
#include <windows.h>
using namespace std;

struct player {
	string name;
	int hp,att,def,spd,max,cur,reg,cost,crit_cnc; 
	float crit_mul;

	// max,cur,reg=maxbattle gauge,current battle gauge,battle gauge regeneration rate/turn
	// cost = 1 att cost
	// spd nanti yang ditampilin 100-spd,spd semakin kecil di source code
	// semakin  besar spdnya,makin sering nyerang, makin gede evade chance, makin gede hit chance
};
	
struct enemy {
	string name;
	int hp,att,def,spd,crit_cnc;
	float crit_mul;
};

void battle1(player& atter,enemy& defer,int times){
	// kalau >200 hitnya, crit chance+10%,100<X<200 +5%, 50<X<100 +3%
	atter.crit_cnc+=10;
	while(times>200){
		int crit=rand() % 100+1;
		int dam_var=rand() % 40 +80; //ini buat variabel damage supaya gak konstan
		if(crit<=atter.crit_cnc-defer.crit_cnc){
			int damage = int((atter.att - defer.def) * atter.crit_mul * dam_var / 100);
			if (damage>1){
				defer.hp -= damage;
				cout << damage <<endl;
				times-=1;
			}
			else{
				defer.hp-=1;
				cout << 1 << endl;
				times-=1;
			}
		}
		else{
			int damage= (atter.att-defer.def)* dam_var / 100;
			if (damage>1){
				defer.hp-=damage;
				cout<< damage<<endl;
				times-=1;
			}
			else{
				defer.hp-=1;
				cout << 1 << endl;
				times-=1;
			}
		}
	}
	atter.crit_cnc-=5;
	while(times>100){
		int crit=rand() % 100+1;
		int dam_var=rand() % 40 +80; //ini buat variabel damage supaya gak konstan
		if(crit<=atter.crit_cnc-defer.crit_cnc){
			int damage = int((atter.att - defer.def) * atter.crit_mul * dam_var / 100);
			if (damage>1){
				defer.hp -= damage;
				cout << damage <<endl;
				times-=1;
			}
			else{
				defer.hp-=1;
				cout << 1 << endl;
				times-=1;
			}
		}
		else{
			int damage = (atter.att-defer.def)* dam_var / 100;
			if (damage>1){
				defer.hp-=damage;
				cout<< damage<<endl;
				times-=1;
			}
			else{
				defer.hp-=1;
				cout << 1 << endl;
				times-=1;
			}
		}
	}
	atter.crit_cnc-=2;
	while(times>50){
		int crit=rand() % 100+1;
		int dam_var=rand() % 40 +80; //ini buat variabel damage supaya gak konstan
		if(crit<=atter.crit_cnc-defer.crit_cnc){
			int damage = int((atter.att - defer.def) * atter.crit_mul * dam_var / 100);
			if (damage>1){
				defer.hp -= damage;
				cout << damage <<endl;
				times-=1;
			}
			else{
				defer.hp-=1;
				cout << 1 << endl;
				times-=1;
			}
		}
		else{
			int damage= (atter.att-defer.def)* dam_var / 100;
			if (damage>1){
				defer.hp-=damage;
				cout<< damage <<endl;
				times-=1;
			}
			else{
				defer.hp-=1;
				cout << 1 << endl;
				times-=1;
			}
		}
	}
	atter.crit_cnc-=3;
	while(times>0){
		int crit=rand() % 100+1;
		int dam_var=rand() % 40 +80; //ini buat variabel damage supaya gak konstan
		if(crit<=atter.crit_cnc-defer.crit_cnc){
			int damage = int((atter.att - defer.def) * atter.crit_mul * dam_var / 100);
			if (damage>1){
				defer.hp -= damage;
				cout << damage <<endl;
				times-=1;
			}
			else{
				defer.hp-=1;
				cout << 1 << endl;
				times-=1;
			}
		}
		else{
			int damage= (atter.att-defer.def)* dam_var / 100;
			if (damage>1){
				defer.hp-=damage;
				cout<< damage <<endl;
				times-=1;
			}
			else{
				defer.hp-=1;
				cout << 1 << endl;
				times-=1;
			}
		}
	}

	if(defer.hp <= 0)
	{
		defer.hp=0;
		cout<<defer.name<<" is dead"<<endl;
		system( "pause");
	}
	Sleep( 1500 );
}

/*
bool test(enemy& atter,player& defer,int times){
		int damage=0;
		atter.crit_cnc+=10;
		while(times>200){
			int crit=rand() % 100+1;
			if(crit<=atter.crit_cnc-defer.crit_cnc){
				damage+=atter.att-defer.def;
				damage+=atter.att-defer.def;
				times-=1;
			}
			else{
				damage+=atter.att-defer.def;
				times-=1;
			}
		}
		atter.crit_cnc-=5;
		while(times>100){
			int crit=rand() % 100+1;
			if(crit<=atter.crit_cnc-defer.crit_cnc){
				damage+=atter.att-defer.def;
				damage+=atter.att-defer.def;
				times-=1;
			}
			else{
				damage+=atter.att-defer.def;
				times-=1;
			}
		}
		atter.crit_cnc-=2;
		while(times>50){
			int crit=rand() % 100+1;
			if(crit<=atter.crit_cnc-defer.crit_cnc){
				damage+=atter.att-defer.def;
				damage+=atter.att-defer.def;
				times-=1;
			}
			else{
				damage+=atter.att-defer.def;
				times-=1;
			}
		}
		atter.crit_cnc-=3;
		while(times>0){
			int crit=rand() % 100+1;
			cout<<crit<<endl;
			if(crit<=atter.crit_cnc-defer.crit_cnc){
				damage+=atter.att-defer.def;
				damage+=atter.att-defer.def;
				times-=1;
			}
			else{
				damage+=atter.att-defer.def;
				times-=1;
			}
		}
		int max=times*2*(atter.att-defer.def);
		float mean=times*(atter.att-defer.def)+times*(atter.crit_cnc-defer.crit_cnc)/100*(atter.att-defer.def);
		float sd;
		sd=(max-mean)/2.326;
		if((damage-mean)/sd==0.674){
			return true;
		}
		else{
			return false;
		}
}
*/
int main(){
	srand ( time(0));
	string filelocation;

	player hero1;
	hero1.name="yamato";
	hero1.hp=435000;
	hero1.att=382;
	hero1.def=281;
	hero1.spd=30;
	hero1.crit_cnc=21;
	hero1.crit_mul=1.5;
	hero1.max=853;
	hero1.reg=73;
	hero1.cost=5;

	enemy ouroboros;
	ouroboros.name="ouroboros";
	ouroboros.hp=320000;
	ouroboros.att=541;
	ouroboros.def=192;
	ouroboros.spd=40;
	ouroboros.crit_cnc=15;
	ouroboros.crit_mul=3.5;

	enemy chronos;
	chronos.name="chronos";
	chronos.hp=180000;
	chronos.att=900;
	chronos.def=200;
	chronos.spd=1;
	chronos.crit_cnc=83;
	chronos.crit_mul=1.2;

	//char1 itu buat di battle doang, monster1,2 juga, harusnya ambil dari database?
	//karena ribet, char1, monster1,2 gak gua bikin sama kek yg di atas, soalnya buat debugging

	player char1;
	char1.name="yamato";
	char1.hp=435000;
	char1.att=9999;
	char1.def=999999;
	char1.spd=30;
	char1.crit_cnc=21;
	char1.crit_mul=1.5;
	char1.max=999999;
	char1.reg=73;
	char1.cost=5;
	char1.cur=char1.max/2;

	enemy monster1;
	monster1.name="ouroboros";
	monster1.hp=3200000;
	monster1.att=541;
	monster1.def=192;
	monster1.spd=40;
	monster1.crit_cnc=15;
	monster1.crit_mul=3.5;

	enemy monster2;
	monster2.name="chronos";
	monster2.hp=1800000;
	monster2.att=900;
	monster2.def=200;
	monster2.spd=7;
	monster2.crit_cnc=83;
	monster2.crit_mul=1.2;

	bool battle=true;
	if(battle){
		int time=0;
		int turn[3];
		turn[0]=hero1.spd;
		turn[1]=ouroboros.spd;
		turn[2]=chronos.spd;

		while ( battle ) {
			while((time!=turn[0])&&(time!=turn[1])&&(time!=turn[2])){
				//delay
				Sleep( 70 );
				
				//delay end
				time++;
			}
			
			if(time==turn[0]){		
				back:
				turn[0]+=hero1.spd;
				char command='z';
				system("CLS");
				cout<<monster1.name<<endl;
				cout<<"HP="<<monster1.hp<<endl<<endl;
				
				cout<<monster2.name<<endl;
				cout<<"HP="<<monster2.hp<<endl<<endl;
				
				cout<<char1.name<<endl;
				cout<<"HP="<<char1.hp<<endl;
				cout<<"battle gauge="<<char1.cur<<endl;
				cout<<"a=attack"<<endl<<"d=defend"<<endl<<"b=back"<<endl;
				
				while((command!='a')&&(command!='d')&&(command!='b')){
					cin>>command;
				}
				
				if(command=='b'){
					goto back;
				}
				
				if(command=='a'){
					cout<<"1 attack cost="<<char1.cost<<endl;
					bool correct=false;
					string input;
					int times;
					int temp1=0,temp2=0,temp3=0;
					//kalau dibawah 50 attack cost=cost
					//kalau 50<X<100 attack cost=cost+1,dst
					//kalau mau lebih nanti dikali aja jadi temp1=(times-50)*2
					//kali ini gua bikin cost, cost+1, cost+3,cost+5
					do {
						cout<<"how many times?"<<endl;
						cin>>input;
						if (input=="b"){
							goto back;
						}
						istringstream (input) >> times; 
						temp1=0;
						temp2=0;
						temp3=0;
						if(times>200){
							temp3=(times-200)*2;
						}
						if(times>100){
							temp2=(times-100)*2;
						}
						if(times>50){
							temp1=(times-50);
						}
					} while( times*char1.cost+temp1+temp2+temp3>=char1.cur );

					char1.cur-= times*char1.cost+temp1+temp2+temp3;
					string target;
					bool attack=false;
					while(not attack){
						cout<<"to?"<<endl;
						cin>>target;
						if(target=="b"){
							goto back;
						}
						if(target==monster1.name){
							attack=true;
							battle1(char1,monster1,times);
							cout<<endl;
						}
						if(target==monster2.name){
							attack=true;
							battle1(char1,monster2,times);
							cout<<endl;
						}
					}
				}
			
				if(command=='d'){
					char1.cur=char1.cur+char1.reg+10;
					continue;
				}
				
				if(monster1.hp==0&&monster2.hp==0){
					battle=false;
					cout<<"you win!!"<<endl;
				}
			}
			
			if((time==turn[1])&&(monster1.hp>0)){
				turn[1]+=monster1.spd;
				system("CLS");
				int crit=rand() % 100+1;
				int dam_var=rand() % 40 +80; //ini buat variabel damage supaya gak konstan
				if(crit<=monster1.crit_cnc-char1.crit_cnc){
					int damage = int((monster1.att - char1.def) * monster1.crit_mul * dam_var / 100);
					if (damage>1){
						char1.hp -= damage;
						cout<<monster1.name<<" turn"<<endl;
						cout<<monster1.name<<" attacked you for"<<damage<< " damage"<<endl;
					}
					else{
						damage=1;
						char1.hp -= damage;
						cout<<monster1.name<<" turn"<<endl;
						cout<<monster1.name<<" attacked you for "<<damage<< " damage"<<endl;
					}
				}
				else{
					int damage= (monster1.att-char1.def)* dam_var / 100;
					if (damage>1){
						char1.hp -= damage;
						cout<<monster1.name<<" turn"<<endl;
						cout<<monster1.name<<" attacked you for"<<damage<< " damage"<<endl;
					}
					else{
						damage=1;
						char1.hp -= damage;
						cout<<monster1.name<<" turn"<<endl;
						cout<<monster1.name<<" attacked you for"<<damage<< " damage"<<endl;
					}
				}
				
				if(char1.hp<=0){
					cout<<"you lose !"<<endl;
					battle=false;
				}
			}
			if(time==turn[2]&&(monster2.hp>0)){
				turn[2]+=monster2.spd;
				system("CLS");
				int crit=rand() % 100+1;
				int dam_var=rand() % 40 +80; //ini buat variabel damage supaya gak konstan
				if(crit<=monster2.crit_cnc-char1.crit_cnc){
					int damage = int((monster2.att - char1.def) * monster2.crit_mul * dam_var / 100);
					if (damage>1){
						char1.hp -= damage;
						cout<<monster2.name<<" turn"<<endl;
						cout<<monster2.name<<" attacked you for "<<damage<< " damage"<<endl;
					}
					else{
						damage=1;
						char1.hp -= damage;
						cout<<monster2.name<<" turn"<<endl;
						cout<<monster2.name<<" attacked you for "<<damage<< " damage"<<endl;
					}
				}
				else{
					int damage= (monster2.att-char1.def)* dam_var / 100;
					if (damage>1){
						char1.hp -= damage;
						cout<<monster2.name<<" turn"<<endl;
						cout<<monster2.name<<" attacked you for "<<damage<< " damage"<<endl;
					}
					else{
						damage=1;
						char1.hp -= damage;
						cout<<monster2.name<<" turn"<<endl;
						cout<<monster2.name<<" attacked you for "<<damage<< " damage"<<endl;;
					}
				}	
				if(char1.hp<=0){
					cout<<"you lose !"<<endl;
					battle=false;
				}
			}
			time++; //supaya kalau di time 50 yg nyerang itu mati, gak infinite loop, gak bakal ganggu turn di time laen kok
		}
	}
	system("pause");
	return 0;
}
