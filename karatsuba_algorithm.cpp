#include<iostream>
#include<algorithm>
using namespace std;
struct binary_num{
    string a;
    binary_num(){
        a="";
    }
    binary_num(string a){
        this->a=a;//this is a pointer to the current object
    }
    void trimzeroes(string &a){
        string res="";
        int flag=0;
        for(int i=0;i<a.length();i++){
            if(a[i]=='1') flag=1;
            if(flag) res.push_back(a[i]);
        }
        if(res!="") a=res;
    }
    void padzeroes(string &a, string &b){
        int n=max(a.length(),b.length());
        while(a.length()<n) a="0"+a;
        while(b.length()<n) b="0"+b;
    }
    binary_num operator+(binary_num b){
        binary_num res = binary_num();
        int n=max(a.length(),b.a.length());
        padzeroes(a,b.a);
        int carry=0;
        for(int i=n-1;i>=0;i--){
            int sum=a[i]+b.a[i]-2*'0';
            //cout<<sum<<" ";
            if(sum==1 || sum==0){
                if(carry==0) res.a.push_back(sum+'0');
                else{
                    if(sum==0) res.a.push_back('1'), carry=0;
                    else res.a.push_back('0'), carry=1;
                }
            } 
            else res.a.push_back('0'+carry), carry=sum-1;
        }
        while(carry>0) res.a.push_back('1'), carry--;
        reverse(res.a.begin(),res.a.end());
        trimzeroes(res.a);
        return res;
    }
    binary_num operator-(binary_num b){
        binary_num res;
        //check for the greater number by comparing them lexicographically
        //make them of equal length by padding zeroes
        padzeroes(a,b.a);
        for(int i=0;i<a.length();i++){
            if(a[i]!=b.a[i]){
                if(a[i]=='1') break;
                else {swap(a,b.a); break;}
            }
        }
        int n=max(a.length(),b.a.length());
        int carry=0;
        for(int i=n-1;i>=0;i--){
            int diff=a[i]-b.a[i];
            //cout<<diff<<" ";
            if(diff==1 or diff==0){
                if(carry==0) res.a.push_back(diff+'0');
                else{
                    if(diff==0) res.a.push_back('1'),carry=1;
                    else res.a.push_back('0'),carry=0;
                }
            } 
            else{
                if(carry==0) res.a.push_back('1'),carry=1;
                else res.a.push_back('0'), carry=1;
            }
        }
        reverse(res.a.begin(),res.a.end());
        trimzeroes(res.a);
        return res;
    }
    binary_num karatsuba(binary_num s){
        binary_num w,x,y,z,f,g,h;
        padzeroes(a,s.a);
        int n=a.length();
        //base case
        if(n==1){
            binary_num res= binary_num();
            int num= (a[0]-'0')*(s.a[0]-'0');
            res.a.push_back(num+'0');
            return res;
        }
        w.a=a.substr(0,n-n/2);
        x.a=a.substr(n-n/2);
        y.a=s.a.substr(0,n-n/2);
        z.a=s.a.substr(n-n/2);
        //recursively call karatsuba 
        f = w.karatsuba(y);
        h = x.karatsuba(z);
        g = (x+w).karatsuba(y+z)-(f+h);
        for(int i=0;i<2*(n/2);i++) f.a.push_back('0');
        for(int i=0;i<n/2;i++) g.a.push_back('0');
        return f+g+h;
    }
    friend binary_num operator*(binary_num a, binary_num b){
        return a.karatsuba(b);
    }
};
int main(){
    freopen("input.txt","r",stdin);
    binary_num s1,s2,res;
    cin>>s1.a>>s2.a;
    res = s1-s2;
    cout<<res.a<<endl;
}