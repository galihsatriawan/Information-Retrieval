#include <bits/stdc++.h>

using namespace std;
#define forVS(it,v) for(vector<string>::iterator it=v.begin();it!=v.end();it++)

class Dokumen{
    public :
        int no;
        string data; //kalimat utuh
        int n;
        vector<string> kump_kata; //kumpulan kata
        map<string,int> kata_doc; //banyak kata per document
        set<string> detect_kata_doc; //kata unik tiap document
        vector<int> v;
        vector<double> tf;
        vector<double> tf_idf;
        Dokumen(int pos){
            no = pos;
        }

};

vector<Dokumen> docs; //seluruh data/document
vector<string> kata_unik;
map<string,double> df,idf;
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
            df.insert({word,0});
            idf.insert({word,0});
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
            df.insert({d,0});
            idf.insert({d,0});
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
    doc.n = doc.kump_kata.size();
    //tambahkan ke kumpulan dokumen
    docs.push_back(doc);
    return doc.kump_kata;
}
void clear_all_vector(){
    for(int j=0;j<docs.size();j++){
            Dokumen *d = &(docs.at(j));
            d->v.clear();
            d->tf.clear();
            d->tf_idf.clear();
    }
}
void vectorize(){
    //hapus seluruhnya terlebih dahulu
    clear_all_vector();
    for(int i=0;i<kata_unik.size();i++){
        string word= kata_unik.at(i);
        int cnt_df =0;
        for(int j=0;j<docs.size();j++){

            Dokumen *d = &(docs.at(j));
            bool ada = d->detect_kata_doc.count(word)>0;
            //Jika suatu kata unik ada di dokumen
            if(ada) cnt_df++;

            int cnt = ada ? d->kata_doc.at(word) : 0; 
            //cout <<word<<"__"<<cnt <<" ";
            d->v.push_back(cnt);
            d->tf.push_back(1.0*cnt/d->n);
        }
        //replace df
        df.erase(word);
        idf.erase(word);

        df.insert({word,cnt_df});
        double t_idf = log10(docs.size()/(1.0*cnt_df)); // 1.0 disini hanya untuk casting ke double
        idf.insert({word,t_idf});

    }
}
/*
  1. Vectorize terlebih dahulu (untuk mendapatkan Vectorization awal dan tf,df,idf)
  2. Count tf * idf
*/
#define For(i,a,n) for(int i=a;i<=n;i++)
void count_tf_idf(){
    for(int i=0;i<kata_unik.size();i++){
        int j;
        string word = kata_unik[i];
        For(j,0,docs.size()-1){
            Dokumen *d = &docs[j];
            // Karena index kata unik sama dengan index di tf nya
            d->tf_idf.push_back((d->tf[i]) * (idf[word])); // push back dimulai dari 0
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
void print_df_idf(string cmd){
    map<string,double> mp;
    if(cmd.compare("df")==0) mp.insert(df.begin(),df.end());
    else mp.insert(idf.begin(),idf.end());
    int banyak = kata_unik.size();
    for(int i = 0;i<banyak;i++){
        if(i==0) cout << "["<<mp[kata_unik[0]];
            else if(i==banyak-1) cout <<","<< mp[kata_unik[banyak-1]]<<"]"<<endl; 
            else cout<<","<<mp[kata_unik[i]];
    } 
}
void print_tf_idf(Dokumen doc,string cmd){
    vector<double> vt;
    if(cmd.compare("tf_idf")==0)  vt= doc.tf_idf;
    else vt = doc.tf;
    int banyak = vt.size();
    // cout<< banyak<<"<--"<<endl;
    for(int i = 0;i<banyak;i++){   
            if(i==0) cout << "["<<vt[0];
            else if(i==banyak-1) cout <<","<< vt[banyak-1]<<"]"<<endl; 
            else cout<<","<<vt[i];
    }
}
void print_all_v_tf_idf(string cmd){

    for(int i =0;i<docs.size();i++){
        // cout << docs.at(i).kump_kata.size()<<"++"<<endl;;
        cout <<"D"<<(i+1)<<":";print_tf_idf(docs.at(i),cmd);
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
#define forVI(it,d) for(vector<int>::iterator it=d.begin();it!=d.end();it++)
#define forVDo(it,d) for(vector<double>::iterator it=d.begin();it!=d.end();it++)
double scalar_doc(Dokumen d,string cmd){
    double temp=0;
    //penjumlahan seluruh kuadrat
    if(cmd.compare("tf_idf")==0){
        forVDo(it,d.tf_idf){
            temp+= (*it)*(*it);
        }
    }
    else{
        forVI(it,d.v){
            temp+= (*it)*(*it);
        }
    }
    
    //cout << temp<<endl;
    return sqrt(temp);
}
double cross_vector(Dokumen a,Dokumen b,string cmd){
    
    double sum=0;
    for(int i=0;i<kata_unik.size();i++){
        if(cmd.compare("v_awal")==0)  sum += (a.v[i]*b.v[i]);
        else sum += (a.tf_idf[i]*b.tf_idf[i]);
    }
    return sum;
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
  add_data("burung kakak tua");
  add_data("nenek sudah tua");
  add_data("burung sudah tua");
  add_data("tua");
  add_data("kakak nenek");

  print_docs();
  print_uniq();
  vectorize();
  count_tf_idf();
  //print_all_vector();
  //print_all_v_tf_idf("tf");
  print_df_idf("df");
  print_df_idf("idf");
  print_all_v_tf_idf("tf_idf");
  
//   cout << "Scalar D1 : "<<scalar_doc(docs[0])<<endl;
    // print_vector(docs.at(0));
  /* 
  double d1=scalar_doc(docs[0],"v_awal"),d3=scalar_doc(docs[2],"v_awal");  
  double similar13 = cross_vector(docs[0],docs[2],"v_awal")/(d1*d3);
  cout <<"Similarity D1 & D3 : "<<  similar13 << endl;
  */

double d1=scalar_doc(docs[0],"tf_idf"),d4=scalar_doc(docs[3],"tf_idf");  
  cout <<"|D1| = "<<d1<<" |D4| ="<<d4<<endl;
  double similar14 = cross_vector(docs[0],docs[3],"tf_idf")/(d1*d4);
  cout <<"Similarity D1 & D4 : "<<  similar14 << endl;
  
 return 0;
}