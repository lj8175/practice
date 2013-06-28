#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class button;

struct button_listener{
	button_listener(){}
	virtual ~button_listener(){}
	virtual void  button_clicked(button* btn)=0;
};

class button{
	private:
		vector<button_listener*> listeners;
		string name;
	public:
		string get_name(){return name;};
		button(string n):name(n){}
		~button(){}
		void add_button_listener(button_listener* bls){
			listeners.push_back(bls);
		}
		void remove_button_listener(button_listener* bls){
			listeners.erase(remove(listeners.begin(),listeners.end(),bls),listeners.end());
		}
		void click(){
			for(vector<button_listener*>::iterator it=listeners.begin();it!=listeners.end();++it){
				button_listener* bls = *it;
				bls->button_clicked(this);
			}
		}
};


class my_button_listener:public button_listener{
private:
	string s;
public:
	my_button_listener(string str):s(str){}
	void button_clicked(button* btn){
		cout<<btn->get_name()<<" "<<s<<endl;
	}
};

int main(){
	button* btn = new button("Button");
	button_listener* bls1 = new my_button_listener("'update' first  listener");
	button_listener* bls2 = new my_button_listener("'update' second listener");
	
	btn->add_button_listener(bls1);
	btn->add_button_listener(bls2);
	btn->click();

	btn->remove_button_listener(bls1);
	btn->click();

	delete btn;
	delete bls1;
	delete bls2;
	return 0;
}

