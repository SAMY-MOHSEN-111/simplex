/// this is simplex method solver ///
/// Github link: https://github.com/SAMY-MOHSEN-111/simplex.git
/// check if you are using the latest version ///
/// this is version 0.0.2 ///
/// Cairo university FCAI ///
/// Made by Samy Mohsen && Youssef Hammam ///
/// Email: engsamymohsen33@gmail.com
/// Stay safe stay healthy xD ///

#include<iostream>
using namespace std;
class simplex{
private:
    int eq_num;
    int var_num;
    int pivot_row_index;
    int pivot_col_index;
    double pivot_element_n;
    double **tableau;
    double **tableau_copy;
    double *obj_fun_coff;
    double *cj;
    string *obj_fun_symbol;
    string *cj_symbol;
    double *zj;
    double *cj_zj;
    double *ratio;

public:
    simplex(){}
    ~simplex(){
        delete [] cj;
        delete [] zj;
        delete [] cj_zj;
        delete [] obj_fun_coff;

        for (int i = 0; i < eq_num; ++i){
            delete [] tableau[i];
            delete [] tableau_copy[i];
        }
        delete [] tableau;
        delete [] tableau_copy;
    }

    void set_eq_num(int val){
        eq_num=val;
    }
    void set_var_num(int val){
            var_num=val;
    }

    void make_tableau();
    void print_tableau();
    void make_obj_fun_coff();
    void make_cj();
    void make_zj();
    void make_cj_jz();
    void calculate_zj();
    void calculate_cj_zj();
    /// if you need them ///
    void print_cj();
    void print_zj();
    void print_cj_zj();
    ///////////////////////
    void pivot_col();
    void pivot_row();
    void pivot_element();
    bool stop();
    void compute_new_row();
    void compute_new_cj();
    void update_tableau();
    /// if you need to know the pivots elements ///
    /// notice that it prints pivot row & col index
    /// pivot element value
    void pivots_print();
    ///////////////////////////////////////////////
    void update_tableau_copy();
    void print_finial_answer();
    void check_negative();
};

void simplex::make_tableau() {

     tableau = new double *[eq_num];
    for (int i = 0; i < eq_num; ++i) {
        tableau[i] = new double [eq_num+var_num+1];
    }
    tableau_copy = new double *[eq_num];
    for (int i = 0; i < eq_num; ++i) {
        tableau_copy[i] = new double [eq_num+var_num+1];
    }

    for (int i = 0; i < eq_num; ++i) {
        cout<<"enter equation number "<<i+1<<" data:-\n";
        for (int j = 0; j < eq_num+var_num+1; ++j) {
            if(j<var_num){
                cout<<"X"<<j+1<<":";
            }else if(j<eq_num+var_num){
                cout<<"S"<<j+1-var_num<<":";
            }else{
                cout<<"Z"<<":";
            }
            cin>>tableau[i][j]; tableau_copy[i][j]=tableau[i][j];
        }
    }
}
void simplex::print_tableau(){

    for (int i = 0; i <eq_num ; ++i) {
        cout<<cj_symbol[i]<<": ";
        for (int j = 0; j <eq_num+var_num+1 ; ++j) {
            cout<<tableau[i][j]<<" ";
        }
        cout<<"\n";
    }
}
void simplex::make_obj_fun_coff() {

    obj_fun_coff = new double [eq_num+var_num];
    obj_fun_symbol = new string [eq_num+var_num];

    cout<<"enter obj function coefficients:-\n";
    for (int i = 0; i < eq_num+var_num; ++i) {
        string tmp="";
        if (i < var_num) {
            cout << "X" << i + 1 << ":";
            tmp+="X"+to_string(i+1);
        } else if (i < eq_num+var_num) {
            tmp+="S"+to_string(i+1);
            cout << "S" << i + 1 - var_num << ":";
        }
        cin>>obj_fun_coff[i];
        obj_fun_symbol[i]=tmp;
    }
}
void simplex::make_cj() {
    cj = new double [eq_num];
    cj_symbol = new string [eq_num];

    for (int i = 0; i < eq_num; ++i) {
        cj[i] = 0;
        cj_symbol[i]="S"+to_string(i+1);
    }
}
void simplex::print_cj() {
    for (int i = 0; i < eq_num; ++i) {
        cout<<cj[i]<<" ";
    }
}
void simplex::make_zj() {
    zj = new double [eq_num+var_num];
    for (int i = 0; i < eq_num+var_num; ++i) {
        zj[i] = 0;
    }
}
void simplex::make_cj_jz() {
    cj_zj = new double [eq_num+var_num];
    for (int i = 0; i < eq_num+var_num; ++i) {
        cj_zj[i] = 0;
    }
}
void simplex::calculate_zj() {

    for (int i = 0; i <eq_num+var_num ; ++i) {
        double summation_tmp = 0;
        for (int j = 0; j <eq_num ; ++j) {
            summation_tmp += cj[j]*tableau[j][i];
        }
        zj[i]=summation_tmp;
    }
}
void simplex::print_zj() {
    for (int i = 0; i <eq_num+var_num ; ++i) {
        cout<<zj[i]<<" ";
    }
}
void simplex::calculate_cj_zj() {
    for (int i = 0; i < eq_num+var_num; ++i) {
      cj_zj[i]=obj_fun_coff[i]-zj[i];
    }
}
void simplex::print_cj_zj() {
    for (int i = 0; i < eq_num+var_num; ++i) {
        cout<<cj_zj[i]<<" ";
    }
}
void simplex::pivot_col() {
    double mx=-1;
    int index = 0;
    for (int i = 0; i < eq_num+var_num; ++i) {
        if(cj_zj[i]>mx){mx=cj_zj[i];index=i;}
    }
    pivot_col_index=index;
}
void simplex::pivot_row() {
    ratio = new double [eq_num];
    for (int i = 0; i < eq_num; ++i) {
        //tableau[i][pivot_col_index];
        ratio[i]=tableau[i][eq_num+var_num]/tableau[i][pivot_col_index];
    }
    double mn = 999999999; int index = 0;
    for (int i = 0; i < eq_num; ++i) {
        ///change here
        if(ratio[i]<mn&&ratio[i]>=0){mn=ratio[i]; index = i;}
    }
    pivot_row_index=index;
}
void simplex::pivot_element() {
    pivot_element_n=tableau[pivot_row_index][pivot_col_index];
}
bool simplex::stop() {
    for (int i = 0; i < eq_num + var_num; ++i) {
        if(cj_zj[i]>0)return true;
    }
    return false;
}
void simplex::pivots_print(){
    cout<<pivot_row_index<<"\n";
    cout<<pivot_col_index<<"\n";
    cout<<pivot_element_n<<"\n";
}
void simplex::compute_new_row() {
    pivot_col();
    pivot_row();
    pivot_element();
    for (int i = 0; i < eq_num+var_num+1; ++i) {
        tableau[pivot_row_index][i]/=pivot_element_n;
    }
}
void simplex::compute_new_cj() {
    cj[pivot_row_index]=obj_fun_coff[pivot_col_index];
    cj_symbol[pivot_row_index]=obj_fun_symbol[pivot_col_index];
}

void simplex::update_tableau(){
    for (int i = 0; i < eq_num; ++i) {
        if(i==pivot_row_index)continue;
        for (int j = 0; j < eq_num + var_num + 1; ++j) {
            tableau[i][j]-=(tableau_copy[i][pivot_col_index]*tableau[pivot_row_index][j]);
        }
    }
}
void simplex::update_tableau_copy(){
    for (int i = 0; i < eq_num; ++i) {
        for (int j = 0; j < eq_num + var_num + 1; ++j) {
            tableau_copy[i][j]=tableau[i][j];
        }
    }
}
void simplex::print_finial_answer() {
    double sum = 0;
    for (int i = 0; i < eq_num; ++i) {
        if(cj_symbol[i][0]!='X')cout<<"X"+to_string(i+1)<<" = 0\n";
        else
        cout<<cj_symbol[i]<<" = "<<tableau[i][eq_num+var_num]<<"\n";
    }
    for (int i = 0; i < var_num; ++i) {
        sum+=tableau[i][eq_num+var_num]*cj[i];
    }
    cout<<"profit = "<<sum<<"\n";
}
void simplex::check_negative() {
    for (int i = 0; i < eq_num; ++i) {
        if(tableau[i][eq_num+var_num]<0)tableau[i][eq_num+var_num]=0;
    }
}
int main(){
    /// put the input file full path here ///

    freopen("C:\\Users\\samym\\Desktop\\read.txt","rt",stdin);
    simplex s;
    ///********************************///

    /// put here the number of equations
    s.set_eq_num(2);
    /// put here the number of decisions variables
    s.set_var_num(2);

    ///********************************///
    s.make_tableau();
    s.make_obj_fun_coff();
    s.make_cj();
    s.make_zj();
    s.make_cj_jz();
    ///********************************///
    s.calculate_cj_zj();
    s.compute_new_row();
    s.compute_new_cj();
    s.update_tableau();

    while(s.stop()){
        s.compute_new_cj();
        s.calculate_zj();
        s.calculate_cj_zj();
        s.compute_new_row();
        s.update_tableau();
        s.update_tableau_copy();
    }

    cout<<"\n";
    s.check_negative();
    s.print_tableau();
    s.print_finial_answer();
    return 0;
}
