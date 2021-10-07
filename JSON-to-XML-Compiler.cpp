#include<iostream>
#include<cstring>
#include<stdlib.h>
using namespace std;


char document_content[]="{\"firstName\":\"John\",\"lastName\":\"Smith\",\"age\":25,\"address\":\"21 2nd Street\",\"postalCode\":\"10021\",\"phoneNumber\":\"212 555-1234\",\"fax\":null,\"gender\":\"male\"}";
char *car=document_content;
char lexical_unit_type;
char lexical_unit[20];
char *keywords[3]={"true","false","null"};


struct node{
	string synthesized_attribute;
	string label;
	int production;
	node *first_son;
	node *second_son;
	node *third_son;
	node *fourth_son;
	node *fifth_son;
};


node *root, *current_node, *saved_node;


void lexical_analysis();
void copy(char *,char *);
void move_back();
bool is_number();
void accept(char *,char);
void object();
void members();
void members_extension();
void Pair();
void value();
void elements();
void elements_extension();
void predictive_analysis();
void initialize(node *);
void display(node *);
void evaluate(node *);


void copy(char *start_car,char *end_car){
	int i=0;
	for(char *c=start_car;c<=end_car;c++){
		lexical_unit[i++]=*c;
	}
	lexical_unit[i]='\0';
}


bool is_number(){
	for(int i=0;lexical_unit[i]!='\0';i++)
		if (!(isdigit(lexical_unit[i]) || lexical_unit[i] == '.'))
			return false;
	return true;
}


void lexical_analysis(){
	int state=0;
	char *start_caracter;
	
	while(1){
	
		switch(state){
		case 0:
			if(isalpha(*car) || isdigit(*car) || *car=='-' || *car=='.'){
				if(isdigit(*car)){
					state=2;start_caracter=car;break;
				}
				else{
					state=1;start_caracter=car;break;
				}
			}
			else{
				if (*car == ' '){
					break;
				}
					
				else{
					if(*car=='\0'){lexical_unit[0]='\0';lexical_unit_type=' ';return;}
					else{
						if(*car=='[' || *car==']' || *car=='{' || *car=='}' || *car==':' || *car=='\"' || *car==','){
							lexical_unit[0]=*car;
							lexical_unit[1]='\0';
							lexical_unit_type='s';
							cout<<"lexical unit="<<lexical_unit<<endl;
							car++;
							return;			
						}
						else{
							cout<<"Lexical Error"<<endl;
							exit(0);
						}
					}
				}
			}
			
		case 1:
			if(isalpha(*car) || isdigit(*car) || *car==' ' || *car=='-' || *car=='.'){
				break;
			}
			else{
				copy(start_caracter,car-1);
				lexical_unit_type='t';
				return;
			}
		
		case 2:
			if(isalpha(*car) || isdigit(*car) || *car==' ' || *car=='-' || *car=='.'){
				break;
			}
			else{
				copy(start_caracter,car-1);
				if(is_number())
					lexical_unit_type='n';
				else
					lexical_unit_type='t';
				return;
			}
		}
		car++;
	}
}


void accept(char *symbol,char type){
	lexical_analysis();
	
	switch(type){
	
		case 'm':
			cout<<"case m"<<endl;
			if(strcmp(lexical_unit,symbol)==0){		 
				cout<<"word accepted"<<endl;
				cout<<"lexical unit="<<lexical_unit<<endl;
				return;
			}
			else{
				cout<<"Keyword Error"<<endl;
				exit(0);
			}
			
		case 's':
			cout<<"case s"<<endl;
			if(strcmp(lexical_unit,symbol)==0){		 
				cout<<"separator accepted"<<endl;
				cout<<"lexical unit="<<lexical_unit<<endl;
				return;
			}
			else{
				cout<<"Separator Error"<<endl;
				exit(0);
			}
			
		case 't':
			cout<<"case t"<<endl;
			if(type==lexical_unit_type || is_number()){
				for(int i=0;i<3;i++){
					if(strcmp(lexical_unit,keywords[i])==0){
						 cout<<"String Error: don't use keyword as string"<<endl;
						 exit(0);
					}
				}
			}
			else{
				cout<<"String Error"<<endl;
				exit(0);
			}
			
		case 'n':
			cout<<"case n"<<endl;
			if(type==lexical_unit_type){		 
				cout<<"number accepted"<<endl;
				cout<<"lexical unit="<<lexical_unit<<endl;
				return;
			}
	}
}


void initialize(node *n){
	n->first_son = NULL;
	n->second_son = NULL;
	n->third_son = NULL;
	n->fourth_son = NULL;
	n->fifth_son = NULL;
}


void move_back(){
	for(int i=0;lexical_unit[i]!='\0';i++)
		car--;
}


void object(){
	
	cout<<"Object"<<endl;
	current_node->production = 1;

	accept("{",'s');
	current_node->first_son = new node();
	initialize(current_node->first_son);
	current_node->first_son->label = "{";
	current_node->first_son->production = -1;

	current_node->second_son = new node();
	initialize(current_node->second_son);
	current_node->second_son->label = "Members";
	saved_node = current_node;
	current_node = current_node->second_son;
	members();
	current_node = saved_node;
	
	accept("}",'s');
	current_node->third_son = new node();
	initialize(current_node->third_son);
	current_node->third_son->label = "}";
	current_node->third_son->production = -1;
}


void members(){
	cout<<"Members"<<endl;
	current_node->production = 2;

	current_node->first_son = new node();
	initialize(current_node->first_son);
	current_node->first_son->label = "Pair";
	saved_node = current_node;
	current_node = current_node->first_son;
	Pair();
	current_node = saved_node;

	current_node->second_son = new node();
	initialize(current_node->second_son);
	current_node->second_son->label = "MembersExtension";
	current_node = current_node->second_son;
	members_extension();
}


void members_extension(){
	cout<<"MembersSExtension"<<endl;

	lexical_analysis();
	cout<<"lexical unit="<<lexical_unit<<endl;

	if(strcmp(lexical_unit,",")==0){
		cout<<", accepted"<<endl;
		current_node->production = 3;
		current_node->first_son = new node();
		initialize(current_node->first_son);
		current_node->first_son->label = ",";
		current_node->first_son->production = -1;

		current_node->second_son = new node();
		initialize(current_node->second_son);
		current_node->second_son->label = "Members";
		current_node = current_node->second_son;
		members();	
	}

	else{
		move_back();
		current_node->production = 4;
		current_node->first_son = new node();
		initialize(current_node->first_son);
		current_node->first_son->label = "";
		current_node->first_son->production = -1;	
	}	
}


void Pair(){
	cout<<"Pair"<<endl;
	current_node->production = 5;

	accept("\"",'s');
	current_node->first_son = new node();
	initialize(current_node->first_son);
	current_node->first_son->label = "\"";
	current_node->first_son->production = -1;

	accept(" ",'t');
	current_node->second_son = new node();
	initialize(current_node->second_son);
	current_node->second_son->label = lexical_unit;
	current_node->second_son->production = -1;

	accept("\"",'s');
	current_node->third_son = new node();
	initialize(current_node->third_son);
	current_node->third_son->label = "\"";
	current_node->third_son->production = -1;

	accept(":",'s');
	current_node->fourth_son = new node();
	initialize(current_node->fourth_son);
	current_node->fourth_son->label = ":";
	current_node->fourth_son->production = -1;
	
	current_node->fifth_son = new node();
	initialize(current_node->fifth_son);
	current_node->fifth_son->label = "Value";
	current_node = current_node->fifth_son;
	value();
}


void value(){
	cout<<"Value"<<endl;
	lexical_analysis();
	if(strcmp(lexical_unit,"\"")==0){
		cout<<" \" accepted"<<endl;
		current_node->production = 6;
		current_node->first_son = new node();
		initialize(current_node->first_son);
		current_node->first_son->label = "\"";
		current_node->first_son->production = -1;

		accept(" ",'t');
		current_node->second_son = new node();
		initialize(current_node->second_son);
		current_node->second_son->label = lexical_unit;
		current_node->second_son->production = -1;

		accept("\"",'s');
		current_node->third_son = new node();
		initialize(current_node->third_son);
		current_node->third_son->label = "\"";
		current_node->third_son->production = -1;

	}
	else{
		if(lexical_unit_type=='n'){
			cout<<"number accepted";
			current_node->production = 7;
			current_node->first_son = new node();
			initialize(current_node->first_son);
			current_node->first_son->label = lexical_unit;
			current_node->first_son->production = -1;
			return;
		}
		else{
			if(strcmp(lexical_unit,"true")==0){
				cout<<"true accepted";
				current_node->production = 8;
				current_node->first_son = new node();
				initialize(current_node->first_son);
				current_node->first_son->label ="true";
				current_node->first_son->production = -1;
				return;
			}
			else{
				if(strcmp(lexical_unit,"false")==0){
					cout<<"false accepted";
					current_node->production = 9;
					current_node->first_son = new node();
					initialize(current_node->first_son);
					current_node->first_son->label ="false";
					current_node->first_son->production = -1;
					return;
				}
				else{
					if(strcmp(lexical_unit,"null")==0){
						cout<<"null accepted";
						current_node->production = 10;
						current_node->first_son = new node();
						initialize(current_node->first_son);
						current_node->first_son->label ="null";
						current_node->first_son->production = -1;
						return;
					}
					else{
						if(strcmp(lexical_unit,"[")==0){
							cout<<"[ accepted"<<endl;
							current_node->production = 11;
							current_node->first_son = new node();
							initialize(current_node->first_son);
							current_node->first_son->label ="[";
							current_node->first_son->production = -1;

							current_node->second_son = new node();
							initialize(current_node->second_son);
							current_node->second_son->label = "Elements";
							saved_node = current_node;
							current_node = current_node->second_son;
							elements();
							current_node = saved_node;

							accept("]",'s');
							current_node->third_son = new node();
							initialize(current_node->third_son);
							current_node->third_son->label ="]";
							current_node->third_son->production = -1;
						}

						else{
							move_back();
							current_node->production = 12;
							current_node->first_son = new node();
							initialize(current_node->first_son);
							current_node->first_son->label = "Object";
							current_node = current_node->first_son;
							object();
						}
					}
				}
			}
		}
	}
}


void elements(){	
	cout<<"Elements"<<endl;
	current_node->production = 13;

	current_node->first_son = new node();
	initialize(current_node->first_son);
	current_node->first_son->label = "Value";
	saved_node = current_node;
	current_node = current_node->first_son;
	value();
	current_node = saved_node;

	current_node->second_son = new node();
	initialize(current_node->second_son);
	current_node->second_son->label = "ElementsExtension";
	current_node = current_node->second_son;
	elements_extension();
}


void elements_extension(){	
	cout<<"ElementsExtension"<<endl;
	lexical_analysis();
	cout<<"lexical unit="<<lexical_unit<<endl;
	
	if(strcmp(lexical_unit,",")==0){
		cout<<", accepted"<<endl;
		current_node->production = 14;
		current_node->first_son = new node();
		initialize(current_node->first_son);
		current_node->first_son->label = ",";
		current_node->first_son->production = -1;
		
		current_node->second_son = new node();
		initialize(current_node->second_son);
		current_node->second_son->label = "Elements";
		current_node = current_node->second_son;
		elements();
	}
	else{
		move_back();
		current_node->production = 15;
		current_node->first_son = new node();
		initialize(current_node->first_son);
		current_node->first_son->label = "";
		current_node->first_son->production = -1;
	}
}


void predictive_analysis(){
	root = new node();
	initialize(root);
	current_node = root;
	current_node->label = "Object";
	object();
	
	if(*car=='\0'){
		cout<<"Document content is error-free JSON syntax"<<endl;
	}
	else{
		cout<<"Document content isn't error-free JSON syntax"<<endl;
		exit(0);
	}
}


void display(node *n){
	if (n->production == -1)
		cout<<n->label<<" ";
	else{
		display(n->first_son);
		if (n->second_son != NULL){
			display(n->second_son);
			if (n->third_son != NULL){
				display(n->third_son);
				if (n->fourth_son != NULL){
					display(n->fourth_son);
					if (n->fifth_son != NULL){
						display(n->fifth_son);
					}
				}
			}
		}
	}
}


void evaluate(node *n){
	
	switch (n->production){
		case 1:
			evaluate(n->second_son);
			n->synthesized_attribute = n->second_son->synthesized_attribute;
			break;
		case 2:
			evaluate(n->first_son);
			evaluate(n->second_son);
			n->synthesized_attribute = n->first_son->synthesized_attribute + n->second_son->synthesized_attribute;
			break;
		case 3:
			evaluate(n->second_son);
			n->synthesized_attribute = n->second_son->synthesized_attribute;
			break;
		case 4:
			n->synthesized_attribute = "";
			break;
		case 5:
			evaluate(n->fifth_son);
			n->synthesized_attribute = "<" + n->second_son->label + ">" + n->fifth_son->synthesized_attribute + "</" + n->second_son->label + ">";
			break;
		case 6:
			n->synthesized_attribute = n->second_son->label;
			break;
		case 7:
			n->synthesized_attribute = n->first_son->label;
			break;
		case 8:
			n->synthesized_attribute = n->first_son->label;
			break;
		case 9:
			n->synthesized_attribute = n->first_son->label;
			break;
		case 10:
			n->synthesized_attribute = n->first_son->label;
			break;
		case 11:
			evaluate(n->second_son);
			n->synthesized_attribute = n->second_son->synthesized_attribute;
			break;
		case 12:
			evaluate(n->first_son);
			n->synthesized_attribute = n->first_son->synthesized_attribute;
			break;
		case 13:
			evaluate(n->first_son);
			evaluate(n->second_son);
			n->synthesized_attribute = n->first_son->synthesized_attribute + n->second_son->synthesized_attribute;
			break;
		case 14:
			evaluate(n->second_son);
			n->synthesized_attribute = n->second_son->synthesized_attribute;
			break;
		case 15:
			n->synthesized_attribute = "";
			break;
	}
}


int main(){
	predictive_analysis();
	cout<<endl;
	cout<<"Derivation Tree: " <<endl;
	cout<<endl;
	display(root);
	cout<<endl;
	cout<<endl;
	evaluate(root);
	cout<<"The JSON object transformed into XML syntax is: "<<endl;
	cout<<endl;
	cout<<root->synthesized_attribute;
	cout<<endl;
	return 0;
}


