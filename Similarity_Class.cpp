#include <bits/stdc++.h>

using namespace std;
#define forVS(it,v) for(vector<string>::iterator it=v.begin();it!=v.end();it++)

class Dokumen{
    public :
        int no;
        string data; //kalimat utuh
        vector<string> kump_kata; //kumpulan kata
        map<string,int> kata_doc; //banyak kata per document
        set<string> detect_kata_doc; //kata unik tiap document
        vector<int> v;
        Dokumen(int pos){
            no = pos;
        }
};

vector<Dokumen> docs; //seluruh data/document
vector<string> kata_unik;
set<string> uniq; // Deteksi seluruh kata unik


vector<string> split_sent(Dokumen &doc, char delim){
    string d = doc.data;
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
        }
        //apakah unik bagi documennya
        if(doc.detect_kata_doc.count(word)==0){
           // doc.kump_kata = word;      karena nantinya juga dapat dari hasil return
           doc.detect_kata_doc.insert(word);
            
           doc.kata_doc.insert({word,1}); //awal 
        //    cout<<doc.kata_doc.at(word)<<"--"<<word<<endl;
        }else{
            int banyak = doc.kata_doc.at(word);
            
            // Replace
            doc.kata_doc.erase(word);
            doc.kata_doc.insert({word,banyak+1});        
            // cout<<doc.kata_doc.at(word)<<"--"<<word<<endl;   
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
    //Sama seperti sebelumnya hanya saja khusus word terakhir
    // Apakah unik
        if(uniq.count(d)==0) {
            uniq.insert(d);
            kata_unik.push_back(d);
        }
    //apakah unik bagi documennya 
        if(doc.detect_kata_doc.count(d)==0){
           // doc.kump_kata = word;      karena nantinya juga dapat dari hasil return
           doc.detect_kata_doc.insert(d);
        
           doc.kata_doc.insert({d,1}); //awal 
        }else{
            int banyak = doc.kata_doc.at(d);
            
            // Replace
            doc.kata_doc.erase(d);
            doc.kata_doc.insert({d,banyak+1});           
        }
    return temp;
}
//Return berupa Pecahan kata
vector<string> find_uniq(Dokumen &doc){
    return split_sent(doc,' ');
}
// Parameter berupa string dan akan dibuatkan object Dokumen
//Return berupa Pecahan kata
vector<string> add_data(string d){
    Dokumen doc(docs.size()+1); // No Dokumen 
    doc.data = d;
    doc.kump_kata = find_uniq(doc);
    
    //tambahkan ke kumpulan dokumen
    docs.push_back(doc);
    return doc.kump_kata;
}
void clear_all_vector(){
    for(int j=0;j<docs.size();j++){
            Dokumen *d = &(docs.at(j));
            d->v.clear();
    }
}
void vectorize(){
    //hapus seluruhnya terlebih dahulu
    clear_all_vector();
    for(int i=0;i<kata_unik.size();i++){
        string word= kata_unik.at(i);
        for(int j=0;j<docs.size();j++){

            Dokumen *d = &(docs.at(j));
            bool ada = d->detect_kata_doc.count(word)>0;
            int cnt = ada ? d->kata_doc.at(word) : 0; 
            //cout <<word<<"__"<<cnt <<" ";
            d->v.push_back(cnt);
            
        }
    }
}

void print_kata(Dokumen doc){
    int banyak = doc.kump_kata.size();
    
    for(int i = 0;i<banyak;i++){
        
            if(i==0) cout << "["<<doc.kump_kata.at(0);
            else if(i==banyak-1) cout <<","<< doc.kump_kata.at(banyak-1)<<"]"<<endl; 
            else cout<<","<<doc.kump_kata.at(i);
        
    }
    
}
void print_vector(Dokumen doc){
    int banyak = doc.v.size();
    // cout<< banyak<<"<--"<<endl;
    for(int i = 0;i<banyak;i++){   
            if(i==0) cout << "["<<doc.v.at(0);
            else if(i==banyak-1) cout <<","<< doc.v.at(banyak-1)<<"]"<<endl; 
            else cout<<","<<doc.v.at(i);
    }
}
void print_all_vector(){
    for(int i =0;i<docs.size();i++){
        // cout << docs.at(i).kump_kata.size()<<"++"<<endl;;
        cout <<"D"<<(i+1)<<":";print_vector(docs.at(i));
    }
}
void print_uniq(){
    int banyak =kata_unik.size();
    for(int i=0;i<banyak;i++){
        if(i==0) cout << "["<<kata_unik.at(0);
            else if(i==banyak-1) cout <<","<< kata_unik.at(banyak-1)<<"]"<<endl; 
            else cout<<","<<kata_unik.at(i);
        
    }
    cout <<endl;
}
#define forVD(it,d) for(vector<Dokumen>::iterator it= d.begin();it!=d.end();it++)
void print_docs(){
    vector<Dokumen>::iterator it ;
    int i=1;
    forVD(it,docs){
        cout <<"D"<<(i++)<<":"<< it->data<<endl;;
    }
}
int main(){
    /* Test  Split Sentence
    vector<string> test = split_sent("halo burung",' ');
    vector<string>::iterator ite ;
    
    forVS(ite,test){
        cout << *ite<<endl;
    }
    
    -- Test Add data
   add_data("halo bang galih");
//    cout<<docs.at(0).data;
   for(int i=0;i<docs.size();i++){
     print_kata(docs.at(i));
   }
   
   */
  add_data("halo bang galih bang");
  add_data("halo juga bang");
  add_data("siap bang");
  print_docs();
  print_uniq();
  vectorize();
  print_all_vector();
    // print_vector(docs.at(0));
   
 return 0;
}