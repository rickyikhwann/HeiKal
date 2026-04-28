#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <sstream>

using namespace std;

struct UserProfile {
    string name;
    char gender;
    int age;
    double height;
    double weight;
    int target;
};

struct FoodItem {
    string name;
    double calories;
    bool perPiece;
};

struct Result {
    double bmi;
    double bmr;
    double tdee;
    double runCalories;
    double targetCalories;
};

struct MealLog {
    string foods;
    double calories;
};

UserProfile user;

const string PROFILE_FILE = "profile.txt";
const string HISTORY_FILE = "history.csv";

vector<FoodItem> FOOD_DATABASE = {
    {"Nasi Putih",130,false},
    {"Dada Ayam",165,false},
    {"Telur Rebus",78,true},
    {"Pisang",89,true},
    {"Brokoli",35,false},
    {"Ubi",86,false},
    {"Salmon",208,false},
    {"Tahu Goreng",76,false},
    {"Tempe Goreng",193,false},
    {"Apel",95,true},
    {"Almond",579,false},
    {"Sayur Bayam",23,false},
    {"Roti Gandum",67,true},
    {"Kentang Rebus",87,false}
};

double calculateBMI(double weight,double height){
    double tinggiM = height / 100.0;
    return weight / (tinggiM * tinggiM);
}

double calculateBMR(double weight,double height,int age,char gender){
    double base = (10 * weight) + (6.25 * height) - (5 * age);
    return (toupper(gender) == 'L') ? base + 5 : base - 161;
}

double calculateRunCalories(double weight,double distance){
    return weight * distance * 1.036;
}

double calculateFoodCalories(double calories,bool perPiece,double quantity){
    if(perPiece) return calories * quantity;
    return calories * quantity / 100.0;
}

double calculateDailyTarget(double bmr,int target,double bmi,double runCalories=0){
    double tdee = (bmr * 1.55) + runCalories;

    if(target == 1){
        if(bmi < 18.5) return tdee + 100;
        if(bmi >= 25) return tdee - 500;
        return tdee - 300;
    }

    if(target == 3){
        if(bmi < 18.5) return tdee + 500;
        return tdee + 300;
    }

    return tdee;
}

Result calculateAll(double distance){
    Result r;
    r.bmi = calculateBMI(user.weight,user.height);
    r.bmr = calculateBMR(user.weight,user.height,user.age,user.gender);
    r.runCalories = calculateRunCalories(user.weight,distance);
    r.tdee = (r.bmr * 1.55) + r.runCalories;
    r.targetCalories = calculateDailyTarget(r.bmr,user.target,r.bmi,r.runCalories);
    return r;
}

string getBMICategory(double bmi){
    if(bmi < 18.5) return "Kurus";
    if(bmi < 25) return "Ideal";
    if(bmi < 30) return "Overweight";
    return "Obesitas";
}

string getBMIAdvice(double bmi){
    if(bmi < 18.5) return "Tubuh kamu masih di bawah ideal. Yuk tambah asupan sehat ya.";
    if(bmi < 25) return "Berat badan kamu sudah ideal. Pertahankan terus ya.";
    if(bmi < 30) return "Sedikit overweight, tapi santai. Pelan-pelan pasti turun.";
    return "Kita bisa turunkan berat badanmu perlahan dengan aman.";
}

string getCalorieMotivation(double consumed,double target){
    double percentage = (consumed / target) * 100.0;
    double diff = target - consumed;

    if(consumed == 0) return "Ayo mulai hari ini dengan asupan bernutrisi!";
    if(percentage >= 100 && percentage <= 110) return "Luar biasa! Target kalori harianmu sudah terpenuhi.";
    if(percentage > 110) return "Asupan energi melimpah! Yuk imbangi dengan aktivitas.";
    if(diff > 500) return "Kebutuhan energimu masih banyak. Jangan lupa makan ya!";
    if(diff > 200) return "Sedikit lagi mencapai target. Satu porsi sehat lagi!";
    return "Hampir sampai! Fokus pada nutrisi berkualitas.";
}

void getFoodRecommendations(double remaining){
    cout << "\n=== REKOMENDASI MAKANAN TAMBAHAN ===\n";

    if(remaining <= 0){
        cout << "Kalori harian kamu sudah terpenuhi.\n";
        return;
    }

    cout << "Kamu masih butuh sekitar " << (int)remaining << " kcal\n\n";

    if(remaining <= 150){
        cout << "- Apel merah 1 buah\n";
        cout << "- Yogurt low fat 1 cup\n";
        cout << "- Almond segenggam\n";
    }
    else if(remaining <= 350){
        cout << "- Oatmeal + pisang\n";
        cout << "- Roti gandum + telur\n";
        cout << "- Smoothie berry\n";
    }
    else if(remaining <= 600){
        cout << "- Ikan panggang + salad\n";
        cout << "- Tempe tahu + sayur\n";
        cout << "- Pasta gandum\n";
    }
    else{
        cout << "- Nasi merah + ayam bakar\n";
        cout << "- Gado-gado komplit\n";
        cout << "- Daging + kentang rebus\n";
    }
}

string goalLabel(int target){
    if(target == 1) return "Defisit";
    if(target == 3) return "Surplus";
    return "Ideal";
}

bool loadProfile(){
    ifstream file(PROFILE_FILE);
    if(!file) return false;

    getline(file,user.name);
    file >> user.gender >> user.age >> user.height >> user.weight >> user.target;
    file.close();

    return user.height > 0 && user.weight > 0;
}

void saveProfile(){
    ofstream file(PROFILE_FILE);
    file << user.name << endl;
    file << user.gender << endl;
    file << user.age << endl;
    file << user.height << endl;
    file << user.weight << endl;
    file << user.target << endl;
}

void inputProfile(){
    cin.ignore(1000,'\n');

    cout << "Nama: ";
    getline(cin,user.name);

    cout << "Gender (L/P): ";
    cin >> user.gender;

    cout << "Usia: ";
    cin >> user.age;

    cout << "Tinggi (cm): ";
    cin >> user.height;

    cout << "Berat (kg): ";
    cin >> user.weight;

    cout << "\n1. Defisit\n2. Ideal\n3. Surplus\n";
    cout << "Pilih target: ";
    cin >> user.target;

    saveProfile();
}

void lihatProfile(){
    double bmi = calculateBMI(user.weight,user.height);

    cout << "\n=== PROFIL ===\n";
    cout << "Nama   : " << user.name << endl;
    cout << "Gender : " << user.gender << endl;
    cout << "Usia   : " << user.age << endl;
    cout << "Tinggi : " << user.height << " cm\n";
    cout << "Berat  : " << user.weight << " kg\n";
    cout << "Target : " << goalLabel(user.target) << endl;
    cout << "BMI    : " << fixed << setprecision(2) << bmi
         << " (" << getBMICategory(bmi) << ")\n";
    cout << getBMIAdvice(bmi) << endl;
}

void hitungKalori(){
    double jarak;
    cout << "\nJarak lari (km): ";
    cin >> jarak;

    Result r = calculateAll(jarak);

    cout << fixed << setprecision(2);
    cout << "\n=== HASIL ===\n";
    cout << "BMI          : " << r.bmi << " (" << getBMICategory(r.bmi) << ")\n";
    cout << "BMR          : " << r.bmr << " kcal\n";
    cout << "Kalori lari  : " << r.runCalories << " kcal\n";
    cout << "TDEE         : " << r.tdee << " kcal\n";
    cout << "Target       : " << r.targetCalories << " kcal\n";

    getFoodRecommendations(r.targetCalories);
}

int main(){
    if(!loadProfile()){
        cout << "Input profil terlebih dahulu.\n";
        inputProfile();
    }

    int pilih;
    do{
        cout << "\n=== MENU ===\n";
        cout << "1. Hitung Kalori\n";
        cout << "2. Lihat Profil\n";
        cout << "3. Ubah Profil\n";
        cout << "4. Keluar\n";
        cout << "Pilih: ";
        cin >> pilih;

        switch(pilih){
            case 1: hitungKalori(); break;
            case 2: lihatProfile(); break;
            case 3: inputProfile(); break;
            case 4: cout << "Program selesai.\n"; break;
            default: cout << "Menu tidak valid.\n";
        }

    }while(pilih != 4);

    return 0;
}