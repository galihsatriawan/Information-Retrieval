#include <bits/stdc++.h>

using namespace std;
#define forVS(it,v) for(vector<string>::iterator it=v.begin();it!=v.end();it++)

class dokumen{
    map<string,int> kata_doc;
    set<string> detect_kata_doc;
};

vector<string> data; //seluruh data/document

vector<string> kata_unik;
set<string> uniq; // Deteksi seluruh kata unik

//banyak kata per document
vector<map<string,int>> kata_doc ;
vector<set<string>> detect_kata_doc; //kata unik tiap document



vector<string> split_sent(string d, char delim){
    vector<string> temp;
    int leng = d.length();

    //Cari Posisi Delimiternya
    int pos= d.find(delim);

    /*
    Debugging
    cout << pos <<"--"<<leng<<endl;
    string word=d.substr(0,pos);
    cout << word <<endl;
    d.erase(0,pos+1);
    cout << d <<endl;
    pos = d.find(delim);
    cout << pos <<endl;
    */
    while((pos<leng)&&(pos>0)){
        
        //Ambil kata sebelum delimiter
        string word=d.substr(0,pos);
        // Apakah unik
        if(uniq.count(word)==0) {
            uniq.insert(word);
            kata_unik.push_back(word);
        }else{

        }
        //hapus delimiter
        d.erase(0,pos+1);
        //masukkan ke vector
        temp.push_back(word);
        //cari lagi
        pos = d.find(delim);
    }
    //Last word, pasti tersisa satu
    temp.push_back(d);

    return temp;
}
vector<string> find_uniq(string d){
    return split_sent(d,' ');
}
vector<string> add_data(string d){
    
    data.push_back(d);
    return find_uniq(d);
}

int main(){
    /* Test  Split Sentence
    vector<string> test = split_sent("halo burung",' ');
    vector<string>::iterator ite ;
    
    forVS(ite,test){
        cout << *ite<<endl;
    }
    */
 return 0;
}