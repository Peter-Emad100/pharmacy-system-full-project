//**********Libraries***********//
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>
#include <SFML/System/Clock.hpp>
#include "SaveData.h"
using namespace std;
using namespace sf;
//**********Declarations***********//
extern vector<string> paymentMethods;
const int Size = 100;
int medicine_data = 0;
int user_data = 0;
int requestcounter = 0;
int chosenOption;  // variable to choose with which order you want to go through
// after log in
bool activeDisplay = true;  // Initialize active display to display1
bool activeS1 = true, activeS2 = false, activeS3 = false, activesS4 = false,
activeS5 = false;
bool IsHeAUser = true;
bool issignin = false; //check if i need to switch to sign in page
RenderWindow window(VideoMode(1366, 768), "Your pharmacy", Style::Fullscreen);
Font Calibri;
Texture BackgroundSign;
Texture ButtonTexture, TextTexture, darkbox, textbox, headerbox, Signbox,
buttonup, buttonin;
Texture ButtonEditInfo, ButtonLogOut, ButtonMakeOrder, Buttonsearch,
ButtonViewPrev, backgroundUser;
Texture ButtonManageUser, ButtonManagePay, ButtonManageOrder,
ButtonManageMedicine, backgroundAdmin;
Texture changeButton, WhiteBox;
Texture byName, byCategory, searchBar, resultTable, backgroundsearch;
Texture backgroundShowReceipt, showTable, confirm, price, quantity;
Texture backgroundManageUser, buttonAddUser, buttonRemoveUser, buttonEditUser;
Texture buttonAddMedicine, buttonRemoveMedicine, buttonEditMedicine;
Texture backgroundTex, secbackgroundTex, textboxTex, roleAdminbuttonTex,
roleUserbuttonTex, buttonTex;
string display1, display2;
Text displaytext1, displaytext2;
Text managetext1, managetext2;
string managedisplay1, managedisplay2;
string displayS1, displayS2, displayS3, displayS4, displayS5;
Text displayStext1, displayStext2, displayStext3, displayStext4, displayStext5;
Texture userButton, notuserButton, adminButton, notadminButton;
Texture backgroundMakeOrder, confrimOrder, semitransparent;
string inputUserID, inputMedicineID;
Text inputUserIDText, inputMedicineIDText;
Texture makereq;
Sprite makerequ;
///dont't use this because it will break makeorder if used 
Text Tsearchentered;
string stringsearch;
Text searchID[10], searchName[10], searchQuantity[10], searchCategory[10], searchPrice[10];
Text intitialvalue;
bool searchmakeRequest = false;
// bool and texts for inout displays for edit info (Admin) page ..... don't use it anywhere else to avoid conflicts and glitches
bool editactive1, editactive2, editactive3;
Text editAtext1, editAtext2, editAtext3;
string editAdisplay1, editAdisplay2, editAdisplay3;

bool editUactive;
Text editUtext1, editUtext2;
string editUdisplay1, editUdisplay2;

const Time displayDuration = milliseconds(5000);


struct medicine {
    int ID;
    string name;
    string description;
    string concentration;
    bool availability;
    string category;
    float price;
    int quantity_in_stock;
    void initialize(int _id, string _name, string _desc, string _conc,
        bool _avail, string _category, float _price, int _quantity) {
        ID = _id;
        name = _name;
        description = _desc;
        concentration = _conc;
        availability = _avail;
        category = _category;
        price = _price;
        quantity_in_stock = _quantity;
    }
};
medicine medicines[Size];

struct user {
    enum role { User, Admin };
    int ID;
    string username;
    string password;
    string email;
    string address;
    string phone;
    role his_role;
    void initialize(int _id, string _name, string _pass, string _email,
        string _address, string _phone, role _rol) {
        ID = _id;
        username = _name;
        password = _pass;
        email = _email;
        address = _address;
        phone = _phone;
        his_role = _rol;
    }
};
user users[Size];
user newUser;
user currentUser;  // Temp to keep the current user's data
int currentUser_Index;
struct order {
    int userID;
    string orderDate;
    int orderID;
    bool orderState = false;
    int medicine_ID[10] = {};
    float totalPrice;

    string shipDate;
    void initialize(int _id, string _orderDate, int _medicine_ID[], float _Price,
        string _shipDate, int order_id, bool order_state) {
        userID = _id;
        orderDate = _orderDate;
        totalPrice = _Price;
        shipDate = _shipDate;
        orderID = order_id;
        orderState = order_state;
        int i = 0;
        while (_medicine_ID[i] != 0) {
            medicine_ID[i] = _medicine_ID[i];
            i++;
        }
    }
};
order orders[Size] = {};

struct request {
    int userID;
    string medicineName;
    int amountNeeded;
};
request requests[15];

// All GUI structures

struct StmakeOrder {
    Sprite background;
    Sprite semiTransparent_1, semiTransparent_2, textbox_1, textbox_2, textbox_3,
        confrimOrder;
};
StmakeOrder makeorder;

struct Header {
    Sprite background;
    Text user, pharmacy;
};
Header header;

struct SignUp {
    Sprite background1,
        background2;  // background 1 is for fields ..... background 2 is for sign
    // in instead option
    Sprite buttonup, buttonin;
    Sprite valuefield1, valuefield2, valuefield3, valuefield4,
        valuefield5;  // username, phone num, location, role, new password
    Text user, location, number, role, forgot, password;
    Sprite Background;
    Sprite userButton1, adminButton1, userButton2, adminButton2;
    Text UsernameTaken;
};
SignUp signup;

struct Edit_Info {
    Texture edit_background, change_phone, change_address, make_admin_green,
        make_user_green, make_user_red, make_admin_red, confirm_button, change_user, change_password;

    Sprite background, changePhone, changeAddress, log_out, changeUser, changePass;

    Sprite valuefield1, valuefield2, valuefield3;

    Sprite makeAdminGreen, makeUserGreen, makeAdminRed, makeUserRed, confirm;

    Text input_id, wrng_id;
};
Edit_Info edit_info;

struct strShowAllOrders {
    Sprite background;
    Sprite trans_back;
    Sprite button1;
};
strShowAllOrders ShowAllOrders;

struct AddUsers

{
    Sprite background, secbackground, usernametextbox, addresstextbox,
        emailtextbox, phonetextbox, passwordtextbox, optionsbutton,
        roleAdminbutton, roleUserbutton;

    Text headerText, usernametext, addresstext, emailtext, phonetext,
        passwordtext, roletext, confirmationtext;
};
AddUsers adduser;

struct AddMedicine

{
    Sprite background, secbackground, medNametextbox, medCatagorytextbox,
        medConcentrationtextbox, medPricetextbox, medQuantitytextbox,
        optionsbutton;

    Text medNametext, medCatagorytext, medConcentrationtext, medPricetext,
        medQuantitytext, medConfirmationtext;
};
AddMedicine addmedicine;

struct manageUser {
    Sprite background, semiTransparent, idTextBox, addUser, removeUser, editUser;

    Text Title, userID;
};
manageUser manage_user;

struct manageMedicine {
    Sprite background, semiTransparent, idTextBox, addMedicine, removeMedicine,
        editMedicine;

    Text Title, medicineID;
};
manageMedicine manage_medicine;

struct MedicineInfo {
    Sprite backgroundy;

    Sprite confirm, quantity, price;

    Sprite valuefield1, valuefield2, valuefield3;
};
MedicineInfo medicineinfo;

struct searchMedicine {
    Sprite backgroundx;

    Sprite byName, byCategory, searchBar, resultTable;
};
searchMedicine searchmedicine;

struct showReceipt {
    Sprite backgroundy;

    Sprite showTable, confirm;
};
showReceipt showreceipt;

struct managePayment {
    Sprite background, confirm_button, delete_button, backgroundview;

    Texture managepayment_background, confirm, Delete;

    Sprite valuefield1, valuefield2;

    Text button1, button2, method, numb;

} manage_payment;

struct SignIn {
    Sprite background1,
        background2;  // background 1 is for fields ..... background 2 is for sign
    // in instead option
    Sprite buttonup, buttonin;
    Sprite valuefield1, valuefield2;
    Text user, password, alreadyhave;
    Sprite Background;
};
SignIn signin;

struct userMenu {
    Sprite buttonEditInfo, buttonLogOut, buttonMakeOrder, buttonsearch,
        buttonViewPrev;
    Sprite background;
};
userMenu usermenu;

struct adminMenu {
    Sprite buttonViewPrev, buttonsearch, buttonManageUser, buttonManagePay,
        buttonManageOrder, buttonManageMedicine, buttonLogOut, buttonEditInfo,
        buttonMakeorder;
    Sprite background;
};
adminMenu adminmenu;

struct EditOrderInfo {
    Sprite textboxID, textBoxPrice, semiTransparentBack, changeButton,
        changeButton2, WhiteBox1, WhiteBox2;
    Sprite Background;
    Text OrderID, OrderDetails, MedicineNme, MedicineConcentration, OrderDate,
        OrderState, TotalPrice;
    Text WantChange, OrderState2, TotalPrice2;
};
EditOrderInfo editOrder;

//********Function Declares***********//
void manage_orders(order orders[Size]);
void dataForTestPurposes();
bool isUsernameTaken(string username);
void signUp(string user, string phonenumber, string location, string email,
    string password);
bool validateUser(string username, string password, user& currentUser);
void logInInterface();
void userPermissions();
void adminPermissions();
void editUserCredentials(int index);
bool searchForMedicineByName(string name);
void searchForMedicineByCategory(string category);
void makeOrder(string medicineIDS, string quantity, string payment_method);
void makeOrderFunctional(StmakeOrder& makeorder);
void showOrderReceipt(order lastOrder, string current_time);
void makeRequest(string _username, string _medicineName, int _amountReq);
void showAllPreviousOrders();
void addUser();
void updateUser();
bool removeUser(int userID);
bool removeMedicine(int medID);
void logOut();
void managePaymentMethodes();
void showPaymentMehtode(vector<string> x);
void logInInterface(string username, string password);
//**********GUI FUNCTIONS DECLARATION***********//
void TextureAFonts();

// user menu
void DrawUserMenu(userMenu usermenu);
void SetUserMenu(userMenu& usermenu);
void functioningUserMenu();

void drawShowAllOrders(strShowAllOrders ShowAllOrders);
void setShowAllOrders(strShowAllOrders& ShowAllOrders);

// admin menu
void DrawAdminMenu(adminMenu adminmenu);
void SetAdminMenu(adminMenu& adminmenu);
void functioningAdminMenu();


void DrawSearch(searchMedicine searchmedicine);
void SetSearch(searchMedicine& searchmedicine);
void functioningsearch();

void DrawMakeOrder(StmakeOrder& makeorder);
void SetMakeOrder(StmakeOrder& makeorder);

void DrawShowReceipt(showReceipt showreceipt);
void SetShowReceipt(showReceipt& showreceipt);

void DrawSignUp(SignUp signup);
void SetSignUp(SignUp& signup);
void functioningSignUp();

void setAddMedicine(AddMedicine& addmedicine);
void drawAddMedicine(AddMedicine& addmedicine);

void setAddusers(AddUsers& adduser);
void drawAddusers(AddUsers& adduser);

void SetMedicineEdit(MedicineInfo& medicineinfo);
void MedicineEditShow();
void MedicineEditShowFunctional(bool& medicineEdit, MedicineInfo medicineinfo);

void DrawSignIn(SignIn signin);
void SetSignIn(SignIn& signin);
void functioningSignIn();


void SetMedicineEdit(MedicineInfo& medicineinfo);

void MedicineEditShow();
void MedicineEditShowFunctional(bool& medicineEdit, MedicineInfo medicineinfo);

// Manage User

void set_manageUser(manageUser& manage_user);
void draw_manageUser(manageUser manage_user);
void functioning_manageUser();
// Manage Medicine
void set_manageMedicine(manageMedicine& manage_medicine);
void draw_manageMedicine(manageMedicine manage_medicine);
void functioning_manageMedicine();
// manage payment
void set_managePayment(managePayment& manage_payment);
void Draw_managePayment(managePayment& manage_payment);
void ManagePayment_functional(managePayment& manage_payment);

void showOrderReceipt(order lastOrder, string current_time);
void ShowReceiptFunctional(order lastOrder, bool& show_order_receipt,
    showReceipt showreceipt, string current_time, manageMedicine manage_medicine);

void page_switcher(Header& header, SignUp& signup, SignIn& signin,
    userMenu& usermenu, adminMenu& adminmenu,
    searchMedicine& searchmedicine, showReceipt& showreceipt,
    Edit_Info& edit_info, string current_time,
    StmakeOrder makeorder, manageMedicine manage_medicine);

//edit info pages:-

//user
void Set_EditInfo_User(Edit_Info& edit_info);
void Draw_EditInfo_User(Edit_Info& edit_info);
void EditInfo_User_Functional(Edit_Info& edit_info);



//admin
void Set_EditInfo_Admin(Edit_Info& edit_info);
void Draw_EditInfo_Admin(Edit_Info& edit_info);

void SetEditOrderInfo(EditOrderInfo& edit);
void DrawEditOrderInfo(EditOrderInfo edit);

bool sign_up;
bool show_order_receipt = 0;
int page_num = 0;
bool medicineEdit = 0;
int main() {
    saveAllDataToArr();
    dataForTestPurposes();
    TextureAFonts();
    // sign_up = true;

     // background
    Texture backgroundTexture;
    backgroundTexture.loadFromFile("Assets/pharmacy2.jpg");
    Sprite background;
    background.setTexture(backgroundTexture);
    background.setScale(0.276, 0.218);

    // setting up headers and sign up/in
    // SetHeader(header);
    SetSignUp(signup);
    SetSignIn(signin);
    SetUserMenu(usermenu);
    SetAdminMenu(adminmenu);
    SetSearch(searchmedicine);
    SetShowReceipt(showreceipt);
    Set_EditInfo_Admin(edit_info);
    Set_EditInfo_User(edit_info);
    SetMakeOrder(makeorder);
    SetEditOrderInfo(editOrder);
    setAddusers(adduser);
    setAddMedicine(addmedicine);
    SetMedicineEdit(medicineinfo);
    set_manageUser(manage_user);
    setShowAllOrders(ShowAllOrders);
    set_managePayment(manage_payment);
    set_manageMedicine(manage_medicine);
    // functioningSignUp();
    // window display

    displaytext1.setFont(Calibri);
    displaytext1.setScale(1, 1);
    displaytext1.setPosition(810, 435);
    displaytext1.setFillColor(Color::Black);
    displaytext1.setString(display1);

    //settind display2 :: password
    displaytext2.setFont(Calibri);
    displaytext2.setScale(1, 1);
    displaytext2.setPosition(810, 555);
    displaytext2.setFillColor(Color::Black);
    displaytext2.setString(display2);


    //For signIn function
    //setting display2 :: username
    displaytext1.setFont(Calibri);
    displaytext1.setFont(Calibri);
    displaytext1.setScale(1, 1);
    displaytext1.setScale(1, 1);
    displaytext1.setPosition(810, 435);
    displaytext1.setPosition(810, 435);
    displaytext1.setFillColor(Color::Black);
    displaytext1.setFillColor(Color::Black);
    displaytext1.setString(display1);
    displaytext1.setString(display1);
    //settind display2 :: password
    //setting display2 :: password
    displaytext2.setFont(Calibri);
    displaytext2.setFont(Calibri);
    displaytext2.setScale(1, 1);
    displaytext2.setScale(1, 1);
    displaytext2.setPosition(810, 555);
    displaytext2.setPosition(810, 555);
    displaytext2.setFillColor(Color::Black);
    displaytext2.setFillColor(Color::Black);
    displaytext2.setString(display2);
    displaytext2.setString(display2);
    //For SignUp function
    //setting display1 :: username
    displayStext1.setFont(Calibri);
    displayStext1.setScale(1, 1);
    displayStext1.setPosition(810, 170);
    displayStext1.setFillColor(Color::Black);
    displayStext1.setString(displayS1);
    //setting display2 :: phone num
    TextureAFonts();
    displayStext2.setFont(Calibri);
    set_managePayment(manage_payment);
    displayStext2.setScale(1, 1);
    displayStext2.setPosition(810, 255);
    displayStext2.setFillColor(Color::Black);
    displayStext2.setString(displayS2);
    //setting display3 :: location
    displayStext3.setFont(Calibri);
    displayStext3.setScale(1, 1);
    displayStext3.setPosition(810, 340);
    displayStext3.setFillColor(Color::Black);
    displayStext3.setString(displayS3);
    //setting display4 :: email
    displayStext4.setFont(Calibri);
    displayStext4.setScale(1, 1);
    displayStext4.setPosition(810, 425);
    displayStext4.setFillColor(Color::Black);
    displayStext4.setString(displayS4);
    //setting display5 :: password
    displayStext5.setFont(Calibri);
    displayStext5.setScale(1, 1);
    displayStext5.setPosition(810, 510);
    displayStext5.setFillColor(Color::Black);
    displayStext5.setString(displayS5);

    Event event;
    while (window.pollEvent(event)) {
        /*if (event.type == Event::Closed)
        {
                window.close();
        }*/
        if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
            window.close();
        }
        // createEditMedicineWindow(window);
        /*if (event.type == Event::MouseButtonPressed) {
            // Check if left mouse button is pressed
            if (show_order_receipt) {
                if (event.mouseButton.button == Mouse::Left) {
                    // Get the current mouse position
                    Vector2i mousePosition = Mouse::getPosition(window);
                    // Check if the mouse position intersects with confirm button
                    if (showreceipt.confirm.getGlobalBounds().contains(
                        static_cast<Vector2f>(mousePosition))) {
                        // action performed to get to main menu
                        show_order_receipt = 0;
                        if (currentUser.his_role == user::User) {
                            window.clear();
                            page_num = 2;
                        }
                        else {
                            window.clear();
                            page_num = 3;
                        }
                    }
                }
            }
        }*/

        /*if (event.type == Event::MouseButtonPressed) {
            // Check if left mouse button is pressed
            if (event.mouseButton.button == Mouse::Left) {
                // Get the current mouse position
                Vector2i mousePosition = Mouse::getPosition(window);
                // Check if the mouse position intersects with signup button
                if (sign_up) {
                    if (signup.buttonin.getGlobalBounds().contains(
                        static_cast<Vector2f>(mousePosition))) {
                        // action performed for sign in
                        sign_up = false;
                    }
                }
                else {
                    if (signin.buttonup.getGlobalBounds().contains(
                        static_cast<Vector2f>(mousePosition)) and
                        !sign_up) {
                        sign_up = true;
                    }
                }
            }
        }*/

        //window.clear();
        //ManagePayment_functional(manage_payment);
        // DrawSignUp(signup);
        //window.display();
        /*if (sign_up) {
                DrawSignUp(signup);
                window.draw(displaytext);
        }
        else {
                DrawSignIn(signin);

        }*/
        while (window.isOpen())
        {
            page_switcher(header, signup, signin, usermenu, adminmenu, searchmedicine,
                showreceipt, edit_info, "12:00", makeorder, manage_medicine);
        }

    }
    saveUserDataLocally();
}
//**********Functions***********//

void dataForTestPurposes() {
    //*******************Medicine data****************************
    medicines[0].initialize(1, "Paracetamol", "Pain reliever and fever reducer",
        "500 mg", true, "Analgesic", 5.99, 100);
    medicines[1].initialize(2, "Lisinopril", "Used to treat high blood pressure",
        "10 mg", true, "Antihypertensive", 10.49, 50);
    medicines[2].initialize(3, "Omeprazole",
        "Treats heartburn, stomach ulcers, and "
        "gastroesophageal reflux disease (GERD)",
        "20 mg", true, "Gastrointestinal", 7.25, 80);
    medicines[3].initialize(4, "Atorvastatin",
        "Lowers high cholesterol and triglycerides", "20 mg",
        true, "Lipid-lowering", 15.75, 30);
    medicines[4].initialize(5, "Metformin", "Treats type 2 diabetes", "500 mg",
        true, "Antidiabetic", 8.99, 60);
    medicines[5].initialize(6, "Amoxicillin",
        "Antibiotic used to treat bacterial infections",
        "250 mg", true, "Antibiotic", 6.50, 0);
    medicines[6].initialize(7, "Alprazolam", "Treats anxiety and panic disorders",
        "0.25 mg", true, "Anxiolytic", 12.99, 40);
    medicines[7].initialize(8, "Ibuprofen",
        "Nonsteroidal anti-inflammatory drug (NSAID)",
        "200 mg", true, "Analgesic", 4.75, 200);
    medicines[8].initialize(9, "Cetirizine",
        "Antihistamine used for allergy relief", "10 mg",
        true, "Antihistamine", 9.25, 0);
    medicines[9].initialize(
        10, "Ranitidine",
        "Reduces stomach acid production to treat heartburn and ulcers", "150 mg",
        true, "Gastrointestinal", 6.99, 90);

    //--------------------------------------------------------------------------------------------------------------------------------------------------------

    //*******************User data****************************
    users[0].initialize(1, "Naruto", "NotNaruto", "narutouzumaki@example.com",
        "123 Main St, Cityville", "+1234567890", user::User);
    users[1].initialize(2, "Madara", "password2", "nadarauchiha@example.com",
        "456 Elm St, Townsville", "+1987654321", user::User);
    users[2].initialize(3, "Cillian", "Cillianpass", "callianmurphy@example.com",
        "789 Oak St, Villageton", "+1122334455", user::Admin);
    users[3].initialize(4, "Aras", "passBulut", "arasbulut@example.com",
        "987 Pine St, Hamletville", "+9988776655", user::User);
    users[4].initialize(5, "Sung", "jinwoo", "sungjinwoo@example.com",
        "654 Birch St, Countryside", "+1122334455", user::User);
    users[5].initialize(6, "Iman", "imangadzhi", "imangadzhi@example.com",
        "321 Maple St, Suburbia", "+9988776655", user::User);
    users[6].initialize(7, "Ali", "AliAli", "alimuhammadali.smith@example.com",
        "111 Cedar St, Ruralville", "+1122334455", user::Admin);

    //--------------------------------------------------------------------------------------------------------------------------------------------------------
    int medicine1[] = { 1, 2, 3, 0 };
    orders[0].initialize(1, "2024-03-27", medicine1, 500.0, "2024-03-27", 1,
        false);
    int medicine2[] = { 4, 5, 0 };
    orders[1].initialize(2, "2024-03-28", medicine2, 300.0, "2024-03-28", 2,
        true);
    int medicine3[] = { 9, 4, 5, 7, 0 };
    orders[2].initialize(3, "2024-03-29", medicine3, 3000.0, "2024-03-29", 3,
        true);
}

void signUp(string user, string phonenumber, string location, string email,
    string password) {

    int id = user_data + 1; // Next available ID

    newUser.ID = id;
    newUser.username = user;
    newUser.phone = phonenumber;
    newUser.address = location;
    newUser.password = password;
    newUser.email = email;

    users[id - 1] = newUser; // Save the new user data into our users array

    saveOneUserDataLocally();
    user_data++;  // Increment user_data to keep track of the total number of users

    page_num = 1;

}

void logInInterface() {
    bool loggedIn = false;

    while (!loggedIn) {
        cout << "Enter your username: ";
        cin >> currentUser.username;
        cout << "Enter your password: ";
        cin >> currentUser.password;

        if (validateUser(currentUser.username, currentUser.password, currentUser)) {
            loggedIn = true;
            cout << "Log in success. Welcome back, " << currentUser.username
                << " :D\n-------------------------------------------\n";

            if (currentUser.his_role == user::User) {
                userPermissions();
            }
            else {
                adminPermissions();
            }

            cin >> chosenOption;
        }

        else {
            cout << "Invalid credentials. The username or password you entered is "
                "incorrect. Please try again.\n";
        }
    }
}

void userPermissions() {
    cout << "1- Search for medicine by name\n";
    cout << "2- Search for medicine by category\n";
    cout << "3- Add order\n";
    cout << "4- Choose payment method\n";
    cout << "5- View order\n";
    cout << "6- Request drug\n";
    cout << "7- View all previous orders\n";
    cout << "8- Log out\n";
}

void adminPermissions() {
    cout << "1- Add new user\n";
    cout << "2- Update user information\n";
    cout << "3- Remove user\n";
    cout << "4- Add new medicine\n";
    cout << "5- Remove medicine\n";
    cout << "6- Manage orders\n";
    cout << "7- Manage payments\n";
    cout << "8- Search for medicine by name\n";
    cout << "9- Search for medicine by category\n";
    cout << "10- Add order\n";
    cout << "11- Choose payment method\n";
    cout << "12- View order\n";
    cout << "13- Request drug\n";
    cout << "14- View all previous orders\n";
}

bool searchForMedicineByName(string name) {
    searchmakeRequest = false;
    int x = name.size();
    if (name[0] >= 'a' && name[0] <= 'z') {
        name[0] -= 32;
    }
    int i = 0;
    bool found = 0;
    float y = 300;
    int emptytextarr = 0;
    while (emptytextarr != 10) {
        searchID[emptytextarr].setString("");
        searchQuantity[emptytextarr].setString("");
        searchPrice[emptytextarr].setString("");
        searchName[emptytextarr].setString("");
        searchCategory[emptytextarr].setString("");
        emptytextarr++;
    }
    int textarrindex = 0;
    while (medicines[i].ID != 0) {
        string_view sv(medicines[i].name.c_str(), x);
        if (name == sv) {
            DrawSearch(searchmedicine);
            float x = 20;
            searchID[textarrindex].setFont(Calibri);
            searchID[textarrindex].setString(to_string(medicines[i].ID));
            searchID[textarrindex].setPosition(x, y);
            x += 40;
            searchName[textarrindex].setFont(Calibri);
            searchName[textarrindex].setString(medicines[i].name);
            searchName[textarrindex].setPosition(x, y);
            x += 160;
            searchCategory[textarrindex].setFont(Calibri);
            searchCategory[textarrindex].setString(medicines[i].category);
            searchCategory[textarrindex].setPosition(x, y);
            x += 200;
            searchPrice[textarrindex].setFont(Calibri);
            searchPrice[textarrindex].setString(to_string(medicines[i].price));
            searchPrice[textarrindex].setPosition(x, y);
            x += 160;
            searchQuantity[textarrindex].setFont(Calibri);
            searchQuantity[textarrindex].setString(to_string(medicines[i].quantity_in_stock));
            searchQuantity[textarrindex].setPosition(x, y);
            x += 80;
            y += 40;
            textarrindex++;
            found = 1;
        }
        i++;
    }
    if (found) {
        return 1;
    }
    else {
        //int amountrequested;
        searchmakeRequest = true;
        return 0;
    }
}

void searchForMedicineByCategory(string category) {
    searchmakeRequest = false;
    float y = 300;
    int emptytextarr = 0;
    while (emptytextarr != 10) {
        searchID[emptytextarr].setString("");
        searchQuantity[emptytextarr].setString("");
        searchPrice[emptytextarr].setString("");
        searchName[emptytextarr].setString("");
        searchCategory[emptytextarr].setString("");
        emptytextarr++;
    }
    int textarrindex = 0;
    bool found = false;
    if (category[0] >= 'a' && category[0] <= 'z') {
        category[0] -= 32;
    }
    for (int i = 0; i < Size; i++) {
        if (category == medicines[i].category) {
            DrawSearch(searchmedicine);
            float x = 20;
            searchID[textarrindex].setFont(Calibri);
            searchID[textarrindex].setString(to_string(medicines[i].ID));
            searchID[textarrindex].setPosition(x, y);
            x += 40;
            searchName[textarrindex].setFont(Calibri);
            searchName[textarrindex].setString(medicines[i].name);
            searchName[textarrindex].setPosition(x, y);
            x += 160;
            searchCategory[textarrindex].setFont(Calibri);
            searchCategory[textarrindex].setString(medicines[i].category);
            searchCategory[textarrindex].setPosition(x, y);
            x += 200;
            searchPrice[textarrindex].setFont(Calibri);
            searchPrice[textarrindex].setString(to_string(medicines[i].price));
            searchPrice[textarrindex].setPosition(x, y);
            x += 160;
            searchQuantity[textarrindex].setFont(Calibri);
            searchQuantity[textarrindex].setString(to_string(medicines[i].quantity_in_stock));
            searchQuantity[textarrindex].setPosition(x, y);
            x += 80;
            y += 40;
            textarrindex++;
            found = true;
        }
    }
    if (found == false) {
        //int amountrequested;
        searchmakeRequest = true;
    }

}
// Convert date string to integers
void parseDateString(const std::string& dateString, int& year, int& month,
    int& day) {
    std::stringstream ss(dateString);
    char dash;
    ss >> year >> dash >> month >> dash >> day;
}

// calculate the difference in days between two dates
int dateDifference(const std::string& date1, const std::string& date2) {
    int year1, month1, day1;
    int year2, month2, day2;

    parseDateString(date1, year1, month1, day1);
    parseDateString(date2, year2, month2, day2);

    // Calculate the total number of days for each date
    int days1 = year1 * 365 + month1 * 30 + day1;
    int days2 = year2 * 365 + month2 * 30 + day2;

    // Calculate the difference in days
    int difference = days2 - days1;

    return difference;
}

void makeRequest(string _username, string _medicineName, int _amountReq) {
    _username = currentUser.username;
    int response;
    cout << "Confirm the request? choose (1 for yes / 0 for no): " << endl;
    cin >> response;
    // no cases because it will be handeled in gui
    requestcounter++;
    if (response == 1) {
        for (int i = 0; i < Size; i++) {
            if (_username == users[i].username) {
                currentUser.ID = users[i].ID;
            }
        }
        for (int j = 0; j < 15; j++) {
            if (requests[j].userID == 0) {
                requests[j].userID = currentUser.ID;
                requests[j].medicineName = _medicineName;
                requests[j].amountNeeded = _amountReq;
                break;
            }
        }
    }
}

void showAllPreviousOrders() {
    for (int i = 0; i < Size; i++) {  // checking for the current user ID to be
        // able to get his/her orders using ID
        if (currentUser.username == users[i].username) {
            currentUser.ID == users[i].ID;
            break;
        }
    }

    cout << "Your previous orders: \n";
    cout << "---------------------\n";
    int num_order = 1;  // to display list of numbered orders
    bool found_orders =
        false;  // to check if there were no orders regesitered for this user
    for (int i = 0; i < Size; i++) {
        if (orders[i].userID == currentUser.ID) {
            found_orders = true;
            cout << "Order number (" << num_order << ") : \n";
            cout << "-------------------\n";
            cout << "Date of order: " << orders[i].orderDate << "\n";
            cout << "Ship date: " << orders[i].shipDate << "\n";

            // printing out medicine id

            cout << "Medicine ID: ";
            int j = 0;
            int currentID = 0;

            while (orders[i].medicine_ID[j] != 0) {
                currentID *= 10;
                cout << orders[i].medicine_ID[j];
                j++;
                currentID += orders[i].medicine_ID[j];
            }
            cout << "\n";

            // getting medicine name from medicine ID
            for (int i = 0; i < Size; i++) {
                if (medicines[i].ID == currentID) {
                    cout << "Name of the medicine: " << medicines[i].name << "\n";
                    cout << "concentraion of the medicine" << medicines[i].concentration
                        << "\n";
                    break;
                }
            }

            cout << "Total price: " << orders[i].totalPrice << "\n";
            num_order++;
        }
    }
    if (!found_orders) {
        cout << "You have no previous orders \n";
    }
    cout << "------------------------------------------ \n ";
}




void addUser() {
    int id = user_data + 1;
    newUser.ID = id;

    cout << "Please provide the following information for the new user:\n";
    do {
        cout << "Username: ";
        cin >> newUser.username;

        if (isUsernameTaken(newUser.username))

            cout << "A user with that username already exists. Please enter a "
            "different username: ";

    } while (isUsernameTaken(newUser.username));

    cout << "Password: ";
    cin >> newUser.password;
    cout << "E-mail: ";
    cin >> newUser.email;
    cout << "Address: ";
    cin.ignore(1, '\n');
    getline(cin, newUser.address);
    cout << "Phone Number: ";
    cin >> newUser.phone;

    int roleChoice;

    do {
        cout << "Pick the new role\n1-User\n2-Admin\n";
        cin >> roleChoice;
        if (roleChoice == 1) {
            newUser.his_role = user::User;
        }
        else if (roleChoice == 2) {
            newUser.his_role = user::Admin;
        }
        else {
            cout << "Invalid role choice. Please enter 1 for User or 2 for Admin.\n";
        }
    } while (roleChoice != 1 && roleChoice != 2);

    users[id - 1] = newUser;

    cout << "Congratulations! A new user has been successfully created.\n";

    saveOneUserDataLocally();

    user_data++;  // Increment user_data to keep track of the total number of
    // users
}

void updateUser() {
    int userID;
    bool userFound = false;
    int index = -1;

    while (!userFound) {
        cout << "Enter the ID of the user you are willing to update: ";
        cin >> userID;

        if (userID <= 0) {
            cout << "Invalid ID.\n";
            continue;
        }

        int i = 0;
        while (users[i].ID != 0) {
            if (users[i].ID == userID) {
                userFound = true;
                index = i;
                break;
            }

            ++i;
        }

        if (!userFound) {
            cout << "User not found.\n";
        }
    }

    cout << "---------------------------------\n";
    cout << "Current username: " << users[index].username << "\n";
    cout << "Current password: " << users[index].password << "\n";
    cout << "Current email: " << users[index].email << "\n";
    cout << "Current address: " << users[index].address << "\n";
    cout << "Current phone number: " << users[index].phone << "\n";

    if (users[index].his_role == user::User)
        cout << "Current role: User\n";
    else if (users[index].his_role == user::Admin)
        cout << "Current role: Admin\n";

    char choice;
    int roleChoice;
    do {
        cout << "What do you want to edit?\n";
        cout << "1. Username\n";
        cout << "2. Password\n";
        cout << "3. Email\n";
        cout << "4. Address\n";
        cout << "5. Phone Number\n";
        cout << "6. Role\n\n";
        cout << "Enter your choice: ";

        int option;
        cin >> option;
        string newUsername;
        bool userNameChanged = false;
        switch (option) {
        case 1:

            while (!userNameChanged) {
                cout << "Enter new username: ";
                cin >> newUsername;

                if (newUsername == users[index].username) {
                    cout << "The new username is the same as the current one. Please "
                        "enter a different username.\n";

                }
                else if (isUsernameTaken(newUsername)) {
                    cout << "A user with that username already exists. Please enter a "
                        "different username.\n";

                }
                else {
                    users[index].username = newUsername;
                    cout << "Username updated successfully!\n";
                    userNameChanged = true;
                }
            }
            break;
        case 2:
            cout << "Enter new password: ";
            cin >> users[index].password;
            break;
        case 3:
            cout << "Enter new email: ";
            cin >> users[index].email;
            break;
        case 4:
            cout << "Enter new address: ";
            cin.ignore(1, '\n');
            getline(cin, users[index].address);
            break;
        case 5:
            cout << "Enter new phone number: ";
            cin >> users[index].phone;
            break;
        case 6:
            if (users[index].his_role == user::User) {
                users[index].his_role = user::Admin;
                cout << "Role updated successfully! Your new role is Admin.\n";

            }
            else {
                users[index].his_role = user::User;
                cout << "Role updated successfully! Your new role is User.\n";
            }

            break;

        default:
            cout << "Invalid option!\n";
        }

        cout
            << "Would you like to make any additional edits? Choose between y/n : ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    cout << "User updated successfully!\n";

    saveOneUserDataLocally();
}

bool removeUser(int userID) {
    if (userID <= 0) {

        return false;
    }

    bool found = false;
    for (int i = 0; i < user_data; ++i) {
        if (users[i].ID == userID) {
            users[i].ID = -1;

            found = true;
            saveUserDataLocally();
            return true;
        }
    }

    if (!found) {

        return false;
    }

    return true;
}

bool removeMedicine(int medID) {
    if (medID <= 0) {

        return false;
    }

    bool found = false;
    for (int i = 0; i < medicine_data; ++i) {

        if (medicines[i].ID == -1) {

            return false;
        }
        if (medicines[i].ID == medID) {
            medicines[i].ID = -1;
            found = true;
            saveMedicineDataLocally();
            return true;
        }
    }

    if (!found) {

        return false;
    }


    return true;
}


void trackorder(order orders[]) {
    bool orderfound = false;
    int orderid = -1;
    cout << "enter id : ";
    cin >> orderid;
    for (size_t i = 0; i < 3; i++) {
        if (orderid == orders[i].orderID) {
            if (orders[i].orderState == 0) {
                cout << "OrderNotDelivered" << endl;

            }
            else {
                cout << "OrderDelivered" << endl;
            }
            orderfound = true;
        }
    }
    if (orderfound != true) {
        cout << "order not found enter another id " << endl;
        trackorder(orders);
    }
}

void manage_orders(order orders[Size]) {
    int ID, indx;
    bool found = 0;
    cout << "Enter the orders's ID: ";
    cin >> ID;
    cout << '\n';
    for (int i = 0; i < Size; i++) {
        if (ID == orders[i].orderID) {
            found = 1;
            indx = i;
            break;
        }
    }

    if (found) {  // display the order first
        for (int j = 0; j < 10; j++) {
            if (orders[indx].medicine_ID[j] == 0) {
                break;
            }
            for (int i = 0; i < Size; i++) {
                if (medicines[i].ID == orders[indx].medicine_ID[j]) {
                    cout << "Name of the medicine: " << medicines[i].name << "\n";
                    cout << "concentraion of the medicine" << medicines[i].concentration
                        << "\n";
                    cout << "-----------------------------------------------\n";
                }
            }
        }
        cout << "Order's date: " << orders[indx].orderDate << '\n';
        cout << "Order's state: " << orders[indx].orderState << '\n';
        cout << "Ship date: " << orders[indx].shipDate << '\n';
        cout << "Total price: " << orders[indx].totalPrice << '\n';

    }
    else {
        cout << "There is no order with this ID\n";
        cout << "Please try again\n";
    }
    char answer = 'y';
    do {
        int choice;
        cout << "What do you want to edit?\n";
        cout << "1. Order's state \n";
        cout << "2. Total price \n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            if (orders[indx].orderState == 0) {
                cout << "Order' state is: ";
                if (orders[indx].orderState == 0) {
                    cout << "Not Delivered \n";

                }

                else {
                    cout << "Delivered \n";
                }
            }
            cout << "Do you want to change the order's state ?   choose y/n \n";
            char change;
            cin >> change;
            if (change == 'y' or change == 'Y') {
                if (orders[indx].orderState == 0) {
                    orders[indx].orderState = 1;
                    cout << "Orders's state is now 'Delivered' ";
                }

                else {
                    if (orders[indx].orderState == 1) {
                        orders[indx].orderState = 0;
                        cout << "Orders's state is now 'Not Delivered' ";
                    }
                }
            }
            else {
                break;
            }
            break;

        case 2:
            int new_price;
            cout << "Total price: " << orders[indx].totalPrice << '\n';
            cout << "Enter new price: ";
            cin >> new_price;
            cout << "\n";
            orders[indx].totalPrice = new_price;
            cout << " Total price is now: " << orders[indx].totalPrice << '\n';
            break;
        }

        cout
            << "Would you like to make any additional edits? Choose between y/n : ";
        cin >> answer;

    } while (answer == 'y' || answer == 'Y');
    cout << "Order updated successfully!\n";
}

void logOut() {
    logInInterface();  // Basically, just open the log in interface again if you
    // are willing to log out
}

void TextureAFonts() {
    // inserting images and fonts
    Calibri.loadFromFile("Assets/Calibri.ttf");
    headerbox.loadFromFile("Assets/headerbluegreen.png");
    Signbox.loadFromFile("Assets/Signinupbox.png");
    buttonup.loadFromFile("Assets/Sign IN.png");
    textbox.loadFromFile("Assets/textbox.png");
    buttonin.loadFromFile("Assets/log IN2.png");
    ButtonEditInfo.loadFromFile("Assets/edit_information.png");
    ButtonLogOut.loadFromFile("Assets/log_out.png");
    ButtonMakeOrder.loadFromFile("Assets/make_order.png");
    Buttonsearch.loadFromFile("Assets/search.png");
    ButtonViewPrev.loadFromFile("Assets/view_previous.png");
    backgroundUser.loadFromFile("Assets/BackgroundUser.PNG");
    ButtonManageOrder.loadFromFile("Assets/manage_order.png");
    ButtonManagePay.loadFromFile("Assets/manage_pay.png");
    ButtonManageUser.loadFromFile("Assets/manage_user.png");
    ButtonManageMedicine.loadFromFile("Assets/manage_medicine.png");
    backgroundAdmin.loadFromFile("Assets/BackgroundAdmin.jpg");
    resultTable.loadFromFile("Assets/result_show.png");
    byName.loadFromFile("Assets/by_name.png");
    byCategory.loadFromFile("Assets/by_category.png");
    searchBar.loadFromFile("Assets/bar_search.png");
    backgroundsearch.loadFromFile("Assets/pharmacy3.jpg");
    backgroundShowReceipt.loadFromFile("Assets/pharmacy_order.jpg");
    showTable.loadFromFile("Assets/receipt_back.png");
    confirm.loadFromFile("Assets/confirm.png");
    BackgroundSign.loadFromFile("Assets/pharmacy2.jpg");
    userButton.loadFromFile("Assets/user_sign.png");
    notuserButton.loadFromFile("Assets/red_user_sign.png");
    adminButton.loadFromFile("Assets/admin_sign.png");
    notadminButton.loadFromFile("Assets/red_admin_sign.png");
    WhiteBox.loadFromFile("Assets/WhiteBox.png");
    changeButton.loadFromFile("Assets/change.png");
    backgroundMakeOrder.loadFromFile(
        "Assets/vector-pharmacy-with-doctor-in-counter-drugstore-cartoon-character.jpg");
    confrimOrder.loadFromFile("Assets/confirm_order.png");
    semitransparent.loadFromFile("Assets/receipt_back.png");
    price.loadFromFile("Assets/edit_price.png");
    quantity.loadFromFile("Assets/edit_quantity.png");
    makereq.loadFromFile("Assets/request.png");
    // edit info images
    edit_info.edit_background.loadFromFile(
        "Assets/"
        "pharmacy3.jpg");
    edit_info.change_address.loadFromFile("Assets/change_add.png");
    edit_info.change_phone.loadFromFile("Assets/change_phone.png");
    edit_info.make_admin_green.loadFromFile("Assets/green_make_admin.png");
    edit_info.make_admin_red.loadFromFile("Assets/red_make_admin.png");
    edit_info.make_user_green.loadFromFile("Assets/green_make_user.png");
    edit_info.make_user_red.loadFromFile("Assets/red_make_user.png");
    edit_info.confirm_button.loadFromFile("Assets/confirm.png");
    edit_info.change_user.loadFromFile("Assets/change_user.png");
    edit_info.change_password.loadFromFile("Assets/change_pass.png");

    // adding new user

    backgroundTex.loadFromFile(
        "Assets/"
        "vector-pharmacy-with-doctor-in-counter-drugstore-cartoon-character.jpg");

    secbackgroundTex.loadFromFile("Assets/Signinupbox.png");

    textboxTex.loadFromFile("Assets/textbox.png");

    roleAdminbuttonTex.loadFromFile("Assets/admin_sign.png");

    roleUserbuttonTex.loadFromFile("Assets/user_sign.png");

    buttonTex.loadFromFile("Assets/confirm.png");

    // Manage pages exclusive assets

    backgroundManageUser.loadFromFile(
        "Assets/"
        "pharmacy-with-doctor-in-counter-drugstore-cartoon-character-design-"
        "vector.png");

    buttonAddUser.loadFromFile("Assets/add_user.png");

    buttonRemoveUser.loadFromFile("Assets/delete.png");

    buttonEditUser.loadFromFile("Assets/edit_user.png");

    buttonAddMedicine.loadFromFile("Assets/add_medicine.png");

    buttonEditMedicine.loadFromFile("Assets/edit_medicine.png");

    buttonRemoveMedicine.loadFromFile("Assets/delete.png");

    // Manage order page

    manage_payment.managepayment_background.loadFromFile(
        "Assets/"
        "pharmacy-with-doctor-in-counter-drugstore-cartoon-character-vector "
        "(2).jpg");

    manage_payment.confirm.loadFromFile("Assets/confirm.png");

    manage_payment.Delete.loadFromFile("Assets/delete.png");
}

void DrawHeader(Header header) {
    window.draw(header.background);
    window.draw(header.user);
    window.draw(header.pharmacy);
}
void SetHeader(Header& header) {
    // background of header
    // header.background.setTexture(headerbox);
    // header.background.setOrigin(900 / 2, 650 / 2);
    // header.background.setPosition(190, 0);
    // 61804header.background.setScale(2.5, 0.45);
    // text for header
    header.pharmacy.setFont(Calibri);
    // header.pharmacy.setString("Your Pharmacy..Wherever you are");
    // header.pharmacy.setPosition(300, 10);
    // header.pharmacy.setScale(1, 1);
    // header.pharmacy.setCharacterSize(60);
}

void DrawSignUp(SignUp signup) {
    window.draw(signup.Background);
    window.draw(signup.background1);
    window.draw(signup.valuefield1);
    window.draw(signup.valuefield2);
    window.draw(signup.valuefield3);
    window.draw(signup.valuefield4);
    window.draw(signup.valuefield5);
    window.draw(signup.buttonup);
    window.draw(signup.location);
    window.draw(signup.user);
    window.draw(signup.number);
    window.draw(signup.role);
    window.draw(signup.password);
    window.draw(signup.background2);
    window.draw(signup.forgot);
    window.draw(signup.buttonin);

    if (IsHeAUser == true) {
        window.draw(signup.adminButton1);
        window.draw(signup.userButton1);
    }
    else {
        window.draw(signup.adminButton2);
        window.draw(signup.userButton2);
    }
}
void SetSignUp(SignUp& signup) {
    // Background
    signup.Background.setTexture(BackgroundSign);
    signup.Background.setScale(0.276, 0.218);
    // background 1 located behind value fields
    signup.background1.setTexture(Signbox);
    signup.background1.setPosition(650, 156.2);
    signup.background1.setScale(0.7, 1.25);
    // valuefield1 -> username
    signup.valuefield1.setTexture(textbox);
    signup.valuefield1.setScale(0.6, 0.4);
    signup.valuefield1.setPosition(800, 165);
    // valuefield2-> phone num
    signup.valuefield2.setTexture(textbox);
    signup.valuefield2.setScale(0.6, 0.4);
    signup.valuefield2.setPosition(800, 250);
    // valuefield3->location
    signup.valuefield3.setTexture(textbox);
    signup.valuefield3.setScale(0.6, 0.4);
    signup.valuefield3.setPosition(800, 335);
    // valuefield4->email
    signup.valuefield4.setTexture(textbox);
    signup.valuefield4.setScale(0.6, 0.4);
    signup.valuefield4.setPosition(800, 420);
    // valuefield5-> new password
    signup.valuefield5.setTexture(textbox);
    signup.valuefield5.setScale(0.6, 0.4);
    signup.valuefield5.setPosition(800, 505);

    // button for sign up
    signup.buttonup.setTexture(buttonup);
    signup.buttonup.setScale(0.28, 0.24);
    signup.buttonup.setPosition(890, 665);

    // texts for info needed in sign up
    signup.user.setFont(Calibri);
    signup.user.setString("Username:");
    signup.user.setPosition(680, 180);
    signup.user.setScale(0.8, 0.7);
    signup.number.setFont(Calibri);
    signup.number.setString("Phone num:");
    signup.number.setPosition(680, 265);
    signup.number.setScale(0.8, 0.7);
    signup.location.setFont(Calibri);
    signup.location.setString("Location:");
    signup.location.setPosition(680, 350);
    signup.location.setScale(0.8, 0.7);
    signup.role.setFont(Calibri);
    signup.role.setString("Email:");
    signup.role.setPosition(680, 435);
    signup.role.setScale(0.8, 0.7);
    signup.password.setFont(Calibri);
    signup.password.setString("Password:");
    signup.password.setPosition(680, 520);
    signup.password.setScale(0.8, 0.7);

    // role button
    signup.adminButton1.setTexture(notadminButton);
    signup.adminButton1.setScale(0.25, 0.25);
    signup.adminButton1.setPosition(1050, 580);
    signup.adminButton2.setTexture(adminButton);
    signup.adminButton2.setScale(0.25, 0.25);
    signup.adminButton2.setPosition(1050, 580);

    signup.userButton1.setTexture(userButton);
    signup.userButton1.setScale(0.25, 0.25);
    signup.userButton1.setPosition(820, 580);
    signup.userButton2.setTexture(notuserButton);
    signup.userButton2.setScale(0.25, 0.25);
    signup.userButton2.setPosition(820, 580);

    // log in button
    signup.buttonin.setTexture(buttonin);
    signup.buttonin.setScale(0.28, 0.24);
    signup.buttonin.setPosition(240, 645);

    // text to log in instead if you already have an account
    signup.forgot.setFont(Calibri);
    signup.forgot.setString("Already have an account?");
    signup.forgot.setScale(1, 1);
    signup.forgot.setPosition(170, 560);

    // background 2-> located behind log in button
    signup.background2.setTexture(Signbox);
    signup.background2.setPosition(66, 525);
    signup.background2.setScale(0.55, 0.45);

    signup.UsernameTaken.setFont(Calibri);
    signup.UsernameTaken.setString(
        "Username taken, Please choose another username!");
    signup.UsernameTaken.setScale(1, 1);
    signup.UsernameTaken.setPosition(100, 100);
    signup.UsernameTaken.setFillColor(Color::Red);
}
void functioningSignUp() {

    bool brokenwindow = false;

    issignin = false;
    while (window.isOpen())
    {
        if (brokenwindow)
        {
            break;
        }
        if (issignin) {
            break;
        }
        window.clear();
        DrawSignUp(signup);
        window.draw(displayStext1);
        window.draw(displayStext2);
        window.draw(displayStext3);
        window.draw(displayStext4);
        window.draw(displayStext5);

        window.display();

        Event event;
        while (window.pollEvent(event))
        {
            //if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            //{
                //window.close();
            //}

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {

                Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                // Check if the mouse click is inside the first text field

                if (signup.valuefield1.getGlobalBounds().contains(mousePos)) {

                    activeS1 = true;
                    activeS2 = false;
                    activeS3 = false;
                    activesS4 = false;
                    activeS5 = false;
                }
                else if (signup.valuefield2.getGlobalBounds().contains(mousePos)) {

                    activeS1 = false;
                    activeS2 = true;
                    activeS3 = false;
                    activesS4 = false;
                    activeS5 = false;
                }
                else if (signup.valuefield3.getGlobalBounds().contains(mousePos)) {

                    activeS1 = false;
                    activeS2 = false;
                    activeS3 = true;
                    activesS4 = false;
                    activeS5 = false;
                }
                else if (signup.valuefield4.getGlobalBounds().contains(mousePos)) {

                    activeS1 = false;
                    activeS2 = false;
                    activeS3 = false;
                    activesS4 = true;
                    activeS5 = false;
                }
                else if (signup.valuefield5.getGlobalBounds().contains(mousePos)) {

                    activeS1 = false;
                    activeS2 = false;
                    activeS3 = false;
                    activesS4 = false;
                    activeS5 = true;
                }
                if (signup.adminButton1.getGlobalBounds().contains(mousePos))
                {
                    IsHeAUser = false;
                    newUser.his_role = user::Admin;
                }
                if (signup.userButton2.getGlobalBounds().contains(mousePos))
                {
                    IsHeAUser = true;
                    newUser.his_role = user::User;
                }
                if (signup.buttonin.getGlobalBounds().contains(mousePos))
                {
                    page_num = 1;
                    //brokenwindow = true;
                    //page_switcher(header, signup, signin, usermenu, adminmenu, searchmedicine,
                      //  showreceipt, edit_info, "12:00", makeorder);
                    issignin = true;
                    break;
                }
                if (signup.buttonup.getGlobalBounds().contains(mousePos))
                {
                    if (isUsernameTaken(displayS1))
                    {
                        activeS1 = true;
                        displayS1.resize(0);
                        window.display();

                        sf::RenderWindow window2(sf::VideoMode(400, 200), "Warning!");
                        // Main loop for the second window
                        while (window2.isOpen())
                        {
                            sf::Event event2;
                            while (window2.pollEvent(event2))
                            {
                                if (event2.type == sf::Event::Closed)
                                {
                                    window2.close();
                                }

                            }

                            window2.clear();
                            Text text;
                            text.setFont(Calibri);
                            text.setString("Username taken, Please use another username!");
                            text.setScale(0.5, 0.5);
                            window2.draw(text);
                            // Draw whatever you want in the second window
                            window2.display();
                        }
                    }
                    else {
                        signUp(displayS1, displayS2, displayS3, displayS4, displayS5);
                    }
                }
            }

            if (event.type == Event::TextEntered && isprint(event.text.unicode)) {
                if (activeS1)
                {
                    if (displayS1.size() < 20) {
                        displayS1 += static_cast<char>(event.text.unicode);
                        displayStext1.setString(displayS1);
                        window.draw(displayStext1);
                        window.display();
                    }
                }
                else if (activeS2)
                {
                    if (displayS2.size() < 11) {
                        if (event.text.unicode >= 48 && event.text.unicode <= 57)
                        {
                            displayS2 += static_cast<char>(event.text.unicode);
                            displayStext2.setString(displayS2);
                            window.draw(displayStext2);
                            window.display();
                        }

                    }
                }
                else if (activeS3)
                {
                    if (displayS3.size() < 20) {
                        displayS3 += static_cast<char>(event.text.unicode);
                        displayStext3.setString(displayS3);
                        window.draw(displayStext3);
                        window.display();
                    }
                }
                else if (activesS4)
                {
                    if (displayS4.size() < 20) {
                        displayS4 += static_cast<char>(event.text.unicode);
                        displayStext4.setString(displayS4);
                        window.draw(displayStext4);
                        window.display();
                    }
                }
                else if (activeS5)
                {
                    if (displayS5.size() < 20) {
                        displayS5 += static_cast<char>(event.text.unicode);
                        displayStext5.setString(displayS5);
                        window.draw(displayStext5);
                        window.display();
                    }
                }
            }

            // Handle backspace key
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::BackSpace) {
                if (activeS1) {
                    // Delete the last character from the first text display
                    if (!displayS1.empty()) {
                        displayS1.pop_back();
                        displayStext1.setString(displayS1);
                        window.draw(displayStext1);
                        window.display();
                    }
                }
                else if (activeS2)
                {
                    if (!displayS2.empty()) {
                        displayS2.pop_back();
                        displayStext2.setString(displayS2);
                        window.draw(displayStext2);
                        window.display();
                    }
                }
                else if (activeS3)
                {
                    if (!displayS3.empty()) {
                        displayS3.pop_back();
                        displayStext3.setString(displayS3);
                        window.draw(displayStext3);
                        window.display();
                    }
                }
                else if (activesS4)
                {
                    if (!displayS4.empty()) {
                        displayS4.pop_back();
                        displayStext4.setString(displayS4);
                        window.draw(displayStext4);
                        window.display();
                    }
                }
                else if (activeS5)
                {
                    if (!displayS5.empty()) {
                        displayS5.pop_back();
                        displayStext5.setString(displayS5);
                        window.draw(displayStext5);
                        window.display();
                    }
                }

            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }
        }

    }

}

void DrawSignIn(SignIn signin) {
    window.draw(signin.Background);
    window.draw(signin.background1);
    window.draw(signin.background2);
    window.draw(signin.buttonup);
    window.draw(signin.buttonin);
    window.draw(signin.valuefield1);
    window.draw(signin.valuefield2);
    window.draw(signin.user);
    window.draw(signin.password);
    window.draw(signin.alreadyhave);
}
void SetSignIn(SignIn& signin) {
    //Background
    signin.Background.setTexture(BackgroundSign);
    signin.Background.setScale(0.276, 0.218);

    // background 1 located behind value fields
    signin.background1.setTexture(Signbox);
    signin.background1.setPosition(650, 365.5);
    signin.background1.setScale(0.7, 0.8);

    // valuefield1 -> username
    signin.valuefield1.setTexture(textbox);
    signin.valuefield1.setScale(0.6, 0.4);
    signin.valuefield1.setPosition(800, 430);
    // valuefield2-> phone num
    signin.valuefield2.setTexture(textbox);
    signin.valuefield2.setScale(0.6, 0.4);
    signin.valuefield2.setPosition(800, 550);

    // texts for value fields
    signin.user.setFont(Calibri);
    signin.user.setString("Username:");
    signin.user.setPosition(680, 450);
    signin.user.setScale(0.7, 0.6);
    signin.password.setFont(Calibri);
    signin.password.setString("Password:");
    signin.password.setPosition(680, 570);
    signin.password.setScale(0.7, 0.6);
    // background 2-> located behind sign up button
    signin.background2.setTexture(Signbox);
    signin.background2.setPosition(66, 525);
    signin.background2.setScale(0.55, 0.45);

    // text for sign up
    signin.alreadyhave.setFont(Calibri);
    signin.alreadyhave.setString("Don't have an account?");
    signin.alreadyhave.setPosition(170, 560);
    signin.alreadyhave.setScale(1, 1);
    /// sign up button
    signin.buttonup.setTexture(buttonup);
    signin.buttonup.setScale(0.28, 0.24);
    signin.buttonup.setPosition(235, 645);

    // log in button
    signin.buttonin.setTexture(buttonin);
    signin.buttonin.setScale(0.28, 0.24);
    signin.buttonin.setPosition(890, 630);
}
void functioningSignIn() {
    bool brokenwindow = false;
    window.clear();

    while (window.isOpen()) {

        DrawSignIn(signin);
        window.draw(displaytext1);
        window.draw(displaytext2);
        window.display();

        Event event;

        while (window.pollEvent(event))
        {
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
            {
                window.close();
            }

            // Handle mouse click

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {

                Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                // Check if the mouse click is inside the first text field

                if (signin.valuefield1.getGlobalBounds().contains(mousePos)) {

                    activeDisplay = true;
                }

                // Check if the mouse click is inside the second text field

                if (signin.valuefield2.getGlobalBounds().contains(mousePos)) {

                    activeDisplay = false;
                }

                if (signin.buttonin.getGlobalBounds().contains(mousePos))
                {
                    logInInterface(display1, display2);
                    brokenwindow = true;
                    break;
                }

                if (signin.buttonup.getGlobalBounds().contains(mousePos))
                {
                    page_num = 0;
                    brokenwindow = true;
                    break;
                    // page_switcher(header, signup, signin, usermenu, adminmenu, searchmedicine, showreceipt, edit_info, "12:00", makeorder);
                }
            }

            // Handle text input
             // Handle text input
            if (event.type == Event::TextEntered && isprint(event.text.unicode)) {
                if (activeDisplay) {
                    // Append the entered character to the first text display
                    if (display1.size() < 20) {
                        display1 += static_cast<char>(event.text.unicode);
                        displaytext1.setString(display1);
                    }
                }
                else {
                    // Append the entered character to the second text display
                    if (display2.size() < 20) {
                        display2 += static_cast<char>(event.text.unicode);
                        displaytext2.setString(display2);
                    }
                }

                // Debugging: Print the active display status
                cout << "Active display: " << (activeDisplay ? "display1" : "display2") << endl;
            }

            // Handle backspace key
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::BackSpace) {
                if (activeDisplay) {
                    // Delete the last character from the first text display
                    if (!display1.empty()) {
                        display1.pop_back();
                        displaytext1.setString(display1);

                    }
                }
                else {
                    // Delete the last character from the second text display
                    if (!display2.empty()) {
                        display2.pop_back();
                        displaytext2.setString(display2);

                    }
                }
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }
        }
        if (brokenwindow)
        {
            break;
        }
    }
}

void SetMakeOrder(StmakeOrder& makeorder) {
    // setting background
    makeorder.background.setTexture(backgroundMakeOrder);
    makeorder.background.setScale(0.276, 0.24);
    // setting confirmOrder
    makeorder.confrimOrder.setTexture(confrimOrder);
    makeorder.confrimOrder.setScale(0.4, 0.4);
    makeorder.confrimOrder.setPosition(135, 670);
    // setting semiTransparent1
    makeorder.semiTransparent_1.setTexture(semitransparent);
    makeorder.semiTransparent_1.setScale(2.33, 1.1);
    makeorder.semiTransparent_1.setPosition(0, 135);
    // setting semiTransparent2
    makeorder.semiTransparent_2.setTexture(semitransparent);
    makeorder.semiTransparent_2.setScale(2.7, 1.1);
    makeorder.semiTransparent_2.setPosition(700, 135);
    // setting textbox1
    makeorder.textbox_1.setTexture(textbox);
    makeorder.textbox_1.setScale(0.5, 0.6);
    makeorder.textbox_1.setPosition(75, 180);
    // setting textbox2
    makeorder.textbox_2.setTexture(textbox);
    makeorder.textbox_2.setScale(0.5, 0.6);
    makeorder.textbox_2.setPosition(75, 300);
    // setting texbox3
    makeorder.textbox_3.setTexture(textbox);
    makeorder.textbox_3.setScale(0.5, 0.6);
    makeorder.textbox_3.setPosition(75, 560);
}
void DrawMakeOrder(StmakeOrder& makeorder) {
    int add_y = 33;
    window.draw(makeorder.background);
    window.draw(makeorder.semiTransparent_1);
    window.draw(makeorder.semiTransparent_2);
    window.draw(makeorder.confrimOrder);
    window.draw(makeorder.textbox_1);
    window.draw(makeorder.textbox_2);
    window.draw(makeorder.textbox_3);
    // medicine id text
    Text text;
    text.setFont(Calibri);
    text.setString("medicine IDs");
    text.setPosition(80, 140);
    window.draw(text);
    text.setScale(0.5, 0.5);
    text.setFillColor(sf::Color::Red);
    text.setString("make a space between each medicine ID and another");
    text.setPosition(80, 170);
    window.draw(text);
    // quantity of medicine text
    text.setString("quantity of medicines");
    text.setScale(1, 1);
    text.setFillColor(sf::Color::White);
    text.setPosition(80, 260);
    window.draw(text);
    text.setScale(0.5, 0.5);
    text.setFillColor(sf::Color::Red);
    text.setString("make a space between each quantity and another");
    text.setPosition(80, 290);
    window.draw(text);
    // text for all medicine show
    int x = 730;
    int y = 135;
    text.setScale(1, 1);
    text.setFillColor(sf::Color::White);
    text.setString("ID");
    text.setPosition(x, y);
    window.draw(text);
    y += add_y;
    for (int i = 0; medicines[i].ID != 0; i++) {
        if (medicines[i].ID != -1) {
            text.setString(to_string(medicines[i].ID));
            text.setPosition(x, y);
            window.draw(text);
            y += add_y;
        }
    }
    x += 70;
    y = 135;
    text.setString("name");
    text.setPosition(x, y);
    window.draw(text);
    y += add_y;
    for (int i = 0; medicines[i].ID != 0; i++) {
        if (medicines[i].ID != -1) {
            text.setString(medicines[i].name);
            text.setPosition(x, y);
            window.draw(text);
            y += add_y;
        }
    }
    x += 180;
    y = 135;
    text.setString("price");
    text.setPosition(x, y);
    window.draw(text);
    y += add_y;
    for (int i = 0; medicines[i].ID != 0; i++) {
        if (medicines[i].ID != -1) {
            text.setString(to_string(medicines[i].price) + "$");
            text.setPosition(x, y);
            window.draw(text);

            y += add_y;
        }
    }
    x += 180;
    y = 135;
    text.setString("available");
    text.setPosition(x, y);
    window.draw(text);
    y += add_y;
    for (int i = 0; medicines[i].ID != 0; i++) {
        if (medicines[i].ID != -1) {
            text.setString(to_string(medicines[i].quantity_in_stock));
            text.setPosition(x, y);
            window.draw(text);

            y += add_y;
        }
    }
    // text for payment method
    int c = 80, d = 380;
    text.setString("Choose payment method");
    text.setPosition(c, d);
    window.draw(text);
    d += 30;
    c += 20;
    int i = 1;
    for (auto it = paymentMethods.begin(); it != paymentMethods.end(); ++it) {
        text.setScale(0.7, 0.7);
        text.setString(to_string(i) + ". " + *it);
        text.setPosition(c, d);
        window.draw(text);
        d += 25;
        i++;
    }
}
void makeOrderFunctional(StmakeOrder& makeorder) {
    DrawMakeOrder(makeorder);
    window.display();
}

void SetUserMenu(userMenu& usermenu) {
    // setting background
    usermenu.background.setTexture(backgroundUser);
    usermenu.background.setScale(0.276, 0.24);

    // setting Edit button
    usermenu.buttonEditInfo.setTexture(ButtonEditInfo);
    usermenu.buttonEditInfo.setPosition(100, 170);
    usermenu.buttonEditInfo.setScale(0.7, 0.7);

    // setting LogOut button
    usermenu.buttonLogOut.setTexture(ButtonLogOut);
    usermenu.buttonLogOut.setPosition(1100, 680);
    usermenu.buttonLogOut.setScale(0.3, 0.3);

    // setting Make Order Button
    usermenu.buttonMakeOrder.setTexture(ButtonMakeOrder);
    usermenu.buttonMakeOrder.setPosition(100, 420);
    usermenu.buttonMakeOrder.setScale(0.7, 0.7);

    // setting Search button
    usermenu.buttonsearch.setTexture(Buttonsearch);
    usermenu.buttonsearch.setPosition(860, 170);
    usermenu.buttonsearch.setScale(0.7, 0.7);

    // setting View Previous Orders button
    usermenu.buttonViewPrev.setTexture(ButtonViewPrev);
    usermenu.buttonViewPrev.setPosition(860, 420);
    usermenu.buttonViewPrev.setScale(0.7, 0.7);
}
void DrawUserMenu(userMenu usermenu) {
    window.draw(usermenu.background);
    window.draw(usermenu.buttonEditInfo);
    window.draw(usermenu.buttonLogOut);
    window.draw(usermenu.buttonMakeOrder);
    window.draw(usermenu.buttonsearch);
    window.draw(usermenu.buttonViewPrev);
}
void functioningUserMenu() {
    while (window.isOpen())
    {
        window.clear();
        DrawUserMenu(usermenu);
        window.display();
        bool breaked = false;
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {

                Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                if (usermenu.buttonEditInfo.getGlobalBounds().contains(mousePos))
                {
                    page_num = 7;
                    breaked = true;
                    break;

                }
                if (usermenu.buttonMakeOrder.getGlobalBounds().contains(mousePos))
                {
                    page_num = 8;
                    breaked = true;
                    break;

                }
                if (usermenu.buttonsearch.getGlobalBounds().contains(mousePos))
                {
                    page_num = 4;
                    breaked = true;
                    break;

                }
                if (usermenu.buttonViewPrev.getGlobalBounds().contains(mousePos))
                {
                    page_num = 9;
                    breaked = true;
                    break;
                }
                if (usermenu.buttonLogOut.getGlobalBounds().contains(mousePos))
                {
                    page_num = 1;
                    breaked = true;
                    break;
                }

            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
                window.close();
            }
        }
        if (breaked) {
            break;
        }
    }

}


void SetAdminMenu(adminMenu& adminmenu) {
    // setting background
    adminmenu.background.setTexture(backgroundAdmin);
    adminmenu.background.setScale(0.276, 0.24);

    // setting Edit Info button
    adminmenu.buttonEditInfo.setTexture(ButtonEditInfo);
    adminmenu.buttonEditInfo.setScale(0.5, 0.5);
    adminmenu.buttonEditInfo.setPosition(550, 140);

    // setting Log Out button
    adminmenu.buttonLogOut.setTexture(ButtonLogOut);
    adminmenu.buttonLogOut.setScale(0.28, 0.28);
    adminmenu.buttonLogOut.setPosition(1100, 680);

    // setting Make Order button
    adminmenu.buttonMakeorder.setTexture(ButtonMakeOrder);
    adminmenu.buttonMakeorder.setScale(0.5, 0.5);
    adminmenu.buttonMakeorder.setPosition(550, 270);

    // setting Manage Medicine button
    adminmenu.buttonManageMedicine.setTexture(ButtonManageMedicine);
    adminmenu.buttonManageMedicine.setScale(0.5, 0.5);
    adminmenu.buttonManageMedicine.setPosition(550, 410);

    // setting Manage Order button
    adminmenu.buttonManageOrder.setTexture(ButtonManageOrder);
    adminmenu.buttonManageOrder.setScale(0.5, 0.5);
    adminmenu.buttonManageOrder.setPosition(550, 540);

    // setting Manage Pay button
    adminmenu.buttonManagePay.setTexture(ButtonManagePay);
    adminmenu.buttonManagePay.setScale(0.5, 0.5);
    adminmenu.buttonManagePay.setPosition(950, 140);

    // setting Manage User button
    adminmenu.buttonManageUser.setTexture(ButtonManageUser);
    adminmenu.buttonManageUser.setScale(0.5, 0.5);
    adminmenu.buttonManageUser.setPosition(950, 270);

    // setting Search button
    adminmenu.buttonsearch.setTexture(Buttonsearch);
    adminmenu.buttonsearch.setScale(0.5, 0.5);
    adminmenu.buttonsearch.setPosition(950, 410);

    // setting View Previous button
    adminmenu.buttonViewPrev.setTexture(ButtonViewPrev);
    adminmenu.buttonViewPrev.setScale(0.5, 0.5);
    adminmenu.buttonViewPrev.setPosition(950, 540);
}
void DrawAdminMenu(adminMenu adminmenu) {
    window.draw(adminmenu.background);
    window.draw(adminmenu.buttonEditInfo);
    window.draw(adminmenu.buttonLogOut);
    window.draw(adminmenu.buttonMakeorder);
    window.draw(adminmenu.buttonManageMedicine);
    window.draw(adminmenu.buttonManageOrder);
    window.draw(adminmenu.buttonManagePay);
    window.draw(adminmenu.buttonManageUser);
    window.draw(adminmenu.buttonsearch);
    window.draw(adminmenu.buttonViewPrev);
}
void functioningAdminMenu() {
    while (window.isOpen())
    {
        window.clear();
        DrawAdminMenu(adminmenu);
        window.display();
        bool breaked = false;
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {

                Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                if (adminmenu.buttonEditInfo.getGlobalBounds().contains(mousePos))
                {
                    page_num = 6;
                    breaked = true;
                    break;
                }
                if (adminmenu.buttonLogOut.getGlobalBounds().contains(mousePos))
                {
                    page_num = 1;
                    breaked = true;
                    break;
                }
                if (adminmenu.buttonMakeorder.getGlobalBounds().contains(mousePos))
                {
                    page_num = 8;
                    breaked = true;
                    break;
                }
                if (adminmenu.buttonManageMedicine.getGlobalBounds().contains(mousePos))
                {
                    page_num = 11;
                    breaked = true;
                    break;
                }
                if (adminmenu.buttonManageOrder.getGlobalBounds().contains(mousePos))
                {
                    page_num = 13;
                    breaked = true;
                    break;
                }
                if (adminmenu.buttonManagePay.getGlobalBounds().contains(mousePos))
                {
                    page_num = 10;
                    breaked = true;
                    break;
                }
                if (adminmenu.buttonManageUser.getGlobalBounds().contains(mousePos))
                {
                    page_num = 11;
                    breaked = true;
                    break;
                }
                if (adminmenu.buttonsearch.getGlobalBounds().contains(mousePos))
                {
                    page_num = 4;
                    breaked = true;
                    break;
                }
                if (adminmenu.buttonViewPrev.getGlobalBounds().contains(mousePos))
                {
                    page_num = 9;
                    breaked = true;
                    break;
                }
            }
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
                window.close();
            }
        }
        if (breaked == true)
        {
            break;
        }
    }
}


void DrawSearch(searchMedicine searchmedicine) {
    window.draw(searchmedicine.backgroundx);
    window.draw(searchmedicine.searchBar);
    window.draw(searchmedicine.resultTable);
    window.draw(searchmedicine.byCategory);
    window.draw(searchmedicine.byName);
}
void SetSearch(searchMedicine& searchmedicine) {
    // setting background
    searchmedicine.backgroundx.setTexture(backgroundsearch);
    searchmedicine.backgroundx.setScale(0.276, 0.24);
    // setting byname
    searchmedicine.byName.setTexture(byName);
    searchmedicine.byName.setScale(0.332, 0.42);
    searchmedicine.byName.setPosition(184, 181);
    // setting bycategory
    searchmedicine.byCategory.setTexture(byCategory);
    searchmedicine.byCategory.setScale(0.332, 0.42);
    searchmedicine.byCategory.setPosition(0.3, 181);
    // setting searchbar
    searchmedicine.searchBar.setTexture(searchBar);
    searchmedicine.searchBar.setScale(0.69, 0.55);
    searchmedicine.searchBar.setPosition(366.4, 181.5);
    // setting resultTable
    searchmedicine.resultTable.setTexture(resultTable);
    searchmedicine.resultTable.setScale(0.71, 0.49);
    searchmedicine.resultTable.setPosition(0, 254);
}
void functioningsearch()
{
    //setting stringsearch :: username
    Tsearchentered.setFont(Calibri);
    Tsearchentered.setScale(1.5, 1.5);
    Tsearchentered.setFillColor(Color::Black);
    Tsearchentered.setPosition(486.4, 185.5);
    makerequ.setTexture(makereq);
    makerequ.setScale(0.4, 0.4);
    makerequ.setPosition(367, 590);
    Text notfound;
    notfound.setFont(Calibri);
    notfound.setPosition(20, 300);
    notfound.setString("NOT FOUND");
    bool breaked = false;
    while (window.isOpen()) {

        window.clear();
        DrawSearch(searchmedicine);
        window.draw(Tsearchentered);
        float x = 20, y = 260;
        Text text;
        text.setFont(Calibri);
        text.setString("ID");
        text.setPosition(x, y);
        window.draw(text);
        //40,160,200,160,80
        x += 40;
        text.setString("Name");
        text.setPosition(x, y);
        window.draw(text);
        x += 160;
        text.setString("Category");
        text.setPosition(x, y);
        window.draw(text);
        x += 200;
        text.setString("Price");
        text.setPosition(x, y);
        window.draw(text);
        x += 160;
        text.setString("Quantity");
        text.setPosition(x, y);
        window.draw(text);
        x += 80;
        int textindex = 0;
        while (textindex != 10) {
            window.draw(searchID[textindex]);
            window.draw(searchName[textindex]);
            window.draw(searchCategory[textindex]);
            window.draw(searchQuantity[textindex]);
            window.draw(searchPrice[textindex]);
            textindex++;
        }
        if (searchmakeRequest) {
            window.draw(makerequ);
            window.draw(notfound);
        }
        window.display();

        Event event;

        while (window.pollEvent(event))
        {
            if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
            {
                window.close();
            }
            // Handle text input
            if (event.type == Event::TextEntered && isprint(event.text.unicode)) {

                // Append the entered character to the first text display
                if (stringsearch.size() < 20) {
                    stringsearch += static_cast<char>(event.text.unicode);
                    Tsearchentered.setString(stringsearch);
                    window.draw(Tsearchentered);
                    window.display();
                }
            }

            // Handle backspace key
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::BackSpace) {

                // Delete the last character from the first text display
                if (!stringsearch.empty()) {
                    stringsearch.pop_back();
                    Tsearchentered.setString(stringsearch);
                    window.draw(Tsearchentered);
                    window.display();
                }
            }

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {

                Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                // Check if the mouse click is inside the first text field

                if (searchmedicine.byCategory.getGlobalBounds().contains(mousePos)) {
                    searchForMedicineByCategory(stringsearch);
                }

            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {

                Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                // Check if the mouse click is inside the first text field

                if (searchmedicine.byName.getGlobalBounds().contains(mousePos)) {
                    searchForMedicineByName(stringsearch);
                }

            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {

                Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                // Check if the mouse click is inside the first text field

                if (searchmedicine.byCategory.getGlobalBounds().contains(mousePos)) {
                    searchForMedicineByCategory(stringsearch);
                }

            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {

                Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                // Check if the mouse click is inside the first text field

                if (makerequ.getGlobalBounds().contains(mousePos)) {
                    page_num = 2;
                    breaked = true;
                    break;
                }
            }
        }
        if (breaked) {
            break;

        }
    }
}
void SetShowReceipt(showReceipt& showreceipt) {
    // setting showtable
    showreceipt.showTable.setTexture(showTable);
    showreceipt.showTable.setScale(1.28, 0.8);
    showreceipt.showTable.setPosition(12, 90);
    // setting background
    showreceipt.backgroundy.setTexture(backgroundShowReceipt);
    showreceipt.backgroundy.setScale(0.276, 0.24);
    // setting confirm buttom
    showreceipt.confirm.setTexture(confirm);
    showreceipt.confirm.setScale(0.4, 0.4);
    showreceipt.confirm.setPosition(67, 610);
}
void DrawShowReceipt(showReceipt showreceipt) {
    window.draw(showreceipt.backgroundy);
    window.draw(showreceipt.showTable);
    window.draw(showreceipt.confirm);
}
void ShowReceiptFunctional(order lastOrder, bool& show_order_receipt,
    showReceipt showreceipt, string current_time) {
    Event event;
    show_order_receipt = 1;
    DrawShowReceipt(showreceipt);
    showOrderReceipt(lastOrder, current_time);
    window.display();
}

void Set_EditInfo_User(Edit_Info& edit_info) {
    edit_info.background.setTexture(edit_info.edit_background);
    edit_info.background.setScale(0.276, 0.218);

    edit_info.changeAddress.setTexture(edit_info.change_address);
    edit_info.changeAddress.setScale(0.28, 0.24);
    edit_info.changeAddress.setPosition(600, 300);

    edit_info.changePhone.setTexture(edit_info.change_phone);
    edit_info.changePhone.setScale(0.28, 0.24);
    edit_info.changePhone.setPosition(600, 450);

    // valuefield1 for change adddress input
    edit_info.valuefield1.setTexture(textbox);
    edit_info.valuefield1.setScale(0.6, 0.4);
    edit_info.valuefield1.setPosition(150, 290);

    // valuefield2 for change phone number input
    edit_info.valuefield2.setTexture(textbox);
    edit_info.valuefield2.setScale(0.6, 0.4);
    edit_info.valuefield2.setPosition(150, 440);

    // setting display1 :: addresse

    editUtext1.setFont(Calibri);

    editUtext1.setScale(1, 1);

    editUtext1.setPosition(200, 300);

    editUtext1.setFillColor(Color::Black);

    editUtext1.setString(editUdisplay1);

    // settind display2 :: phone num

    editUtext2.setFont(Calibri);

    editUtext2.setScale(1, 1);

    editUtext2.setPosition(200, 450);

    editUtext2.setFillColor(Color::Black);

    editUtext2.setString(editUdisplay2);

}
void Draw_EditInfo_User(Edit_Info& edit_info) {
    window.draw(edit_info.background);
    window.draw(edit_info.changeAddress);
    window.draw(edit_info.changePhone);
    window.draw(edit_info.valuefield1);
    window.draw(edit_info.valuefield2);
}

void EditInfo_Admin_functional(Edit_Info& edit_info) {
    Set_EditInfo_Admin(edit_info);
    Draw_EditInfo_Admin(edit_info);
}

void Set_EditInfo_Admin(Edit_Info& edit_info) {
    edit_info.background.setTexture(edit_info.edit_background);
    edit_info.background.setScale(0.276, 0.218);

    edit_info.changeUser.setTexture(edit_info.change_user);
    edit_info.changeUser.setScale(0.28, 0.24);
    edit_info.changeUser.setPosition(600, 300);

    edit_info.changePass.setTexture(edit_info.change_password);
    edit_info.changePass.setScale(0.28, 0.24);
    edit_info.changePass.setPosition(600, 450);

    // valuefield1 for change username input
    edit_info.valuefield1.setTexture(textbox);
    edit_info.valuefield1.setScale(0.6, 0.4);
    edit_info.valuefield1.setPosition(150, 290);

    // valuefield2 for change password input
    edit_info.valuefield2.setTexture(textbox);
    edit_info.valuefield2.setScale(0.6, 0.4);
    edit_info.valuefield2.setPosition(150, 440);

    // valuefield3 to input the ID of the user that i want to edit
    edit_info.valuefield3.setTexture(textbox);
    edit_info.valuefield3.setScale(0.4, 0.4);
    edit_info.valuefield3.setPosition(270, 150);

    // make user red and green buttons
    edit_info.makeUserGreen.setTexture(edit_info.make_user_green);
    edit_info.makeUserGreen.setScale(0.28, 0.24);
    edit_info.makeUserGreen.setPosition(200, 555);

    edit_info.makeUserRed.setTexture(edit_info.make_user_red);
    edit_info.makeUserRed.setScale(0.28, 0.24);
    edit_info.makeUserRed.setPosition(200, 555);

    // make admin red and green buttons
    edit_info.makeAdminGreen.setTexture(edit_info.make_admin_green);
    edit_info.makeAdminGreen.setScale(0.28, 0.24);
    edit_info.makeAdminGreen.setPosition(500, 555);

    edit_info.makeAdminRed.setTexture(edit_info.make_admin_red);
    edit_info.makeAdminRed.setScale(0.28, 0.24);
    edit_info.makeAdminRed.setPosition(500, 555);

    // text for ID
    edit_info.input_id.setFont(Calibri);
    edit_info.input_id.setString("Enter ID: ");
    edit_info.input_id.setScale(1, 1);
    edit_info.input_id.setFillColor(Color::Black);
    edit_info.input_id.setPosition(150, 160);

    // confirm button for ID
    edit_info.confirm.setTexture(edit_info.confirm_button);
    edit_info.confirm.setScale(0.28, 0.24);
    edit_info.confirm.setPosition(600, 160);

    //set text for changing username field
    editAtext1.setFont(Calibri);
    editAtext1.setScale(1, 1);
    editAtext1.setPosition(180, 300);
    editAtext1.setFillColor(Color::Black);
    editAtext1.setString(editAdisplay1);
    // set text for changing password field
    editAtext2.setFont(Calibri);
    editAtext2.setScale(1, 1);
    editAtext2.setPosition(180, 453);
    editAtext2.setFillColor(Color::Black);
    editAtext2.setString(editAdisplay2);
    // set text for id input
    editAtext3.setFont(Calibri);
    editAtext3.setScale(1, 1);
    editAtext3.setPosition(320, 160);
    editAtext3.setFillColor(Color::Black);
    editAtext3.setString(editAdisplay3);
    //text when wrong Id is entered
    edit_info.wrng_id.setFont(Calibri);
    edit_info.wrng_id.setString("ID not found, please try again ");
    edit_info.wrng_id.setScale(0.7, 0.7);
    edit_info.wrng_id.setFillColor(Color::Black);
    edit_info.wrng_id.setPosition(280, 205);
}
void Draw_EditInfo_Admin(Edit_Info& edit_info) {

    int userIndex = 0;


    Event event;
    while (window.isOpen()) {
        window.clear();
        window.draw(edit_info.background);
        window.draw(edit_info.changeUser);
        window.draw(edit_info.changePass);
        window.draw(edit_info.valuefield1);
        window.draw(edit_info.valuefield2);
        window.draw(edit_info.valuefield3);
        window.draw(edit_info.input_id);
        window.draw(edit_info.confirm);
        window.draw(editAtext1);
        window.draw(editAtext2);
        window.draw(editAtext3);

        window.display();
        while (window.pollEvent(event)) {

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }


            Vector2i mousePosition = Mouse::getPosition(window);




            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                // Check if the mouse click is inside the first text field
                if (edit_info.valuefield1.getGlobalBounds().contains(mousePos)) {
                    editactive1 = true;
                    editactive2 = false;
                    editactive3 = false;
                }
                else if (edit_info.valuefield2.getGlobalBounds().contains(mousePos)) {
                    editactive1 = false;
                    editactive2 = true;
                    editactive3 = false;
                }
                else if (edit_info.valuefield3.getGlobalBounds().contains(mousePos)) {
                    editactive1 = false;
                    editactive2 = false;
                    editactive3 = true;
                }
            }
            if (event.type == Event::TextEntered && isprint(event.text.unicode)) {
                if (editactive1)
                {
                    if (editAdisplay1.size() < 20) {
                        editAdisplay1 += static_cast<char>(event.text.unicode);
                        editAtext1.setString(editAdisplay1);
                        window.draw(editAtext1);
                        window.display();
                    }
                }
                else if (editactive2)
                {
                    if (editAdisplay2.size() < 20) {
                        editAdisplay2 += static_cast<char>(event.text.unicode);
                        editAtext2.setString(editAdisplay2);
                        window.draw(editAtext2);
                        window.display();
                    }
                }
                else if (editactive3)
                {
                    if (editAdisplay3.size() < 11) {
                        if (event.text.unicode >= 48 && event.text.unicode <= 57)
                        {
                            editAdisplay3 += static_cast<char>(event.text.unicode);
                            editAtext3.setString(editAdisplay3);
                            window.draw(editAtext3);
                            window.display();
                        }
                    }
                }
            }
            // deleting characters -> backspace
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::BackSpace) {
                if (editactive1) {
                    // Delete the last character from the first text display
                    if (!editAdisplay1.empty()) {
                        editAdisplay1.pop_back();
                        editAtext1.setString(editAdisplay1);
                        window.draw(editAtext1);
                        window.display();
                    }
                }
                else if (editactive2)
                {
                    if (!editAdisplay2.empty()) {
                        editAdisplay2.pop_back();
                        editAtext2.setString(editAdisplay2);
                        window.draw(editAtext2);
                        window.display();
                    }
                }
                else if (editactive3)
                {
                    if (!editAdisplay3.empty()) {
                        editAdisplay3.pop_back();
                        editAtext3.setString(editAdisplay3);
                        window.draw(editAtext3);
                        window.display();
                    }
                }
            }
            Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
            if (edit_info.confirm.getGlobalBounds().contains((mousePos))) {
                // Loop through the users until a user with userID = 0 is found,
           // indicating that there are no more users in our database
                bool found=false;
                while (users[userIndex].ID != 0) {
                    found = false;
                    if (users[userIndex].username == editAdisplay3) {
                        found = true;
                        break;
                    }
                    userIndex++;
                }
               
                Clock clock;
                if (!found) {
                    
                    clock.restart();
                    Time elapsedTime = clock.getElapsedTime();
                    while (elapsedTime < displayDuration) {
                        window.draw(edit_info.wrng_id);
                        elapsedTime = clock.getElapsedTime();
                    }
                 
                    
                }
                if (found) {
                    // draws buttons with respect to the role of that user

                    Vector2i mousePosition = Mouse::getPosition(window);

                    if (edit_info.changeUser.getGlobalBounds().contains(
                        static_cast<Vector2f>(mousePosition))) {
                        users[userIndex].username = editAdisplay1;
                    }
                    if (edit_info.changePass.getGlobalBounds().contains(
                        static_cast<Vector2f>(mousePosition))) {
                        users[userIndex].password = editAdisplay2;
                    }


                    while (users[userIndex].his_role == user::User) {

                        window.draw(edit_info.makeUserGreen);
                        window.draw(edit_info.makeAdminRed);
                        window.display();

                        // change user to admin
                        if (edit_info.makeAdminRed.getGlobalBounds().contains(
                            static_cast<Vector2f>(mousePosition))) {
                            users[userIndex].his_role = user::Admin;

                        }
                    }

                    while (users[userIndex].his_role == user::Admin) {
                        window.draw(edit_info.makeAdminGreen);
                        window.draw(edit_info.makeUserRed);
                        window.display();

                        // change admin to user
                        if (edit_info.makeUserRed.getGlobalBounds().contains(
                            static_cast<Vector2f>(mousePosition))) {
                            users[userIndex].his_role = user::User;

                        }
                    }
                }
            }
        }
    }
}

void setAddMedicine(AddMedicine& addmedicine)

{
    // buttons

    addmedicine.optionsbutton.setTexture(buttonTex);

    addmedicine.optionsbutton.setScale(0.39, 0.39);

    addmedicine.optionsbutton.setPosition(590, 630);

    // textboxes

    addmedicine.medNametextbox.setTexture(textboxTex);

    addmedicine.medNametextbox.setScale(0.6, 0.6);

    addmedicine.medNametextbox.setPosition(450, 160);

    addmedicine.medCatagorytextbox.setTexture(textboxTex);

    addmedicine.medCatagorytextbox.setScale(0.6, 0.6);

    addmedicine.medCatagorytextbox.setPosition(450, 250);

    addmedicine.medConcentrationtextbox.setTexture(textboxTex);

    addmedicine.medConcentrationtextbox.setScale(0.6, 0.6);

    addmedicine.medConcentrationtextbox.setPosition(450, 340);

    addmedicine.medPricetextbox.setTexture(textboxTex);

    addmedicine.medPricetextbox.setScale(0.6, 0.6);

    addmedicine.medPricetextbox.setPosition(450, 430);

    addmedicine.medQuantitytextbox.setTexture(textboxTex);

    addmedicine.medQuantitytextbox.setScale(0.6, 0.6);

    addmedicine.medQuantitytextbox.setPosition(450, 520);

    // medicine Name

    addmedicine.medNametext.setFont(Calibri);

    addmedicine.medNametext.setString("Name:");

    addmedicine.medNametext.setPosition(100, 168);

    addmedicine.medNametext.setFillColor(sf::Color::White);

    addmedicine.medNametext.setCharacterSize(55);

    // medicineCatagory

    addmedicine.medCatagorytext.setFont(Calibri);

    addmedicine.medCatagorytext.setString("Catagory:");

    addmedicine.medCatagorytext.setPosition(100, 258);

    addmedicine.medCatagorytext.setFillColor(sf::Color::White);

    addmedicine.medCatagorytext.setCharacterSize(55);

    // medicine Concentraion

    addmedicine.medConcentrationtext.setFont(Calibri);

    addmedicine.medConcentrationtext.setString("Concentration:");

    addmedicine.medConcentrationtext.setPosition(100, 348);

    addmedicine.medConcentrationtext.setFillColor(sf::Color::White);

    addmedicine.medConcentrationtext.setCharacterSize(55);

    // medicine price

    addmedicine.medPricetext.setFont(Calibri);

    addmedicine.medPricetext.setString("Price:");

    addmedicine.medPricetext.setPosition(100, 438);

    addmedicine.medPricetext.setFillColor(sf::Color::White);

    addmedicine.medPricetext.setCharacterSize(55);

    // medicines Quantity

    addmedicine.medQuantitytext.setFont(Calibri);

    addmedicine.medQuantitytext.setString("Quantity:");

    addmedicine.medQuantitytext.setPosition(100, 528);

    addmedicine.medQuantitytext.setFillColor(sf::Color::White);

    addmedicine.medQuantitytext.setCharacterSize(55);

    // confirmation

    addmedicine.medConfirmationtext.setFont(Calibri);

    addmedicine.medConfirmationtext.setString("If input completed click confirm");

    addmedicine.medConfirmationtext.setPosition(120, 655);

    addmedicine.medConfirmationtext.setCharacterSize(30);

    addmedicine.medConfirmationtext.setFillColor(sf::Color::Black);

    // options background

    addmedicine.secbackground.setTexture(secbackgroundTex);

    addmedicine.secbackground.setPosition(65, 140);

    addmedicine.secbackground.scale(0.9, 1.3);

    // main background

    addmedicine.background.setTexture(backgroundTex);

    addmedicine.background.setPosition(0, 0);

    addmedicine.background.setScale(0.28, 0.28);
}
void drawAddMedicine(AddMedicine& addmedicine)

{
    window.draw(addmedicine.background);

    window.draw(addmedicine.secbackground);

    window.draw(addmedicine.medNametextbox);

    window.draw(addmedicine.medNametext);

    window.draw(addmedicine.medCatagorytextbox);

    window.draw(addmedicine.medCatagorytext);

    window.draw(addmedicine.medPricetextbox);

    window.draw(addmedicine.medPricetext);

    window.draw(addmedicine.medConcentrationtextbox);

    window.draw(addmedicine.medConcentrationtext);

    window.draw(addmedicine.medQuantitytextbox);

    window.draw(addmedicine.medQuantitytext);

    window.draw(addmedicine.medConfirmationtext);

    window.draw(addmedicine.optionsbutton);
}

void setAddusers(AddUsers& adduser)

{
    // buttons

    adduser.optionsbutton.setTexture(buttonTex);

    adduser.optionsbutton.setScale(0.39, 0.39);

    adduser.optionsbutton.setPosition(590, 650);

    adduser.roleAdminbutton.setTexture(roleAdminbuttonTex);

    adduser.roleAdminbutton.setScale(0.35, 0.35);

    adduser.roleAdminbutton.setPosition(455, 570);

    adduser.roleUserbutton.setTexture(roleUserbuttonTex);

    adduser.roleUserbutton.setScale(0.35, 0.35);

    adduser.roleUserbutton.setPosition(670, 570);

    // textboxes

    adduser.usernametextbox.setTexture(textboxTex);

    adduser.usernametextbox.setScale(0.6, 0.6);

    adduser.usernametextbox.setPosition(450, 160);

    adduser.addresstextbox.setTexture(textboxTex);

    adduser.addresstextbox.setScale(0.6, 0.6);

    adduser.addresstextbox.setPosition(450, 240);

    adduser.emailtextbox.setTexture(textboxTex);

    adduser.emailtextbox.setScale(0.6, 0.6);

    adduser.emailtextbox.setPosition(450, 320);

    adduser.passwordtextbox.setTexture(textboxTex);

    adduser.passwordtextbox.setScale(0.6, 0.6);

    adduser.passwordtextbox.setPosition(450, 400);

    adduser.phonetextbox.setTexture(textboxTex);

    adduser.phonetextbox.setScale(0.6, 0.6);

    adduser.phonetextbox.setPosition(450, 480);

    // texts for later

    // username

    adduser.usernametext.setFont(Calibri);

    adduser.usernametext.setString("Username:");

    adduser.usernametext.setPosition(100, 168);

    adduser.usernametext.setFillColor(sf::Color::White);

    adduser.usernametext.setCharacterSize(55);

    // address

    adduser.addresstext.setFont(Calibri);

    adduser.addresstext.setString("Address:");

    adduser.addresstext.setPosition(100, 248);

    adduser.addresstext.setFillColor(sf::Color::White);

    adduser.addresstext.setCharacterSize(55);

    // email

    adduser.emailtext.setFont(Calibri);

    adduser.emailtext.setString("Email:");

    adduser.emailtext.setPosition(100, 328);

    adduser.emailtext.setFillColor(sf::Color::White);

    adduser.emailtext.setCharacterSize(55);

    // password

    adduser.passwordtext.setFont(Calibri);

    adduser.passwordtext.setString("Password:");

    adduser.passwordtext.setPosition(100, 408);

    adduser.passwordtext.setFillColor(sf::Color::White);

    adduser.passwordtext.setCharacterSize(55);

    // phone

    adduser.phonetext.setFont(Calibri);

    adduser.phonetext.setString("Password:");

    adduser.phonetext.setPosition(100, 488);

    adduser.phonetext.setFillColor(sf::Color::White);

    adduser.phonetext.setCharacterSize(55);

    // role

    adduser.roletext.setFont(Calibri);

    adduser.roletext.setString("Role:");

    adduser.roletext.setPosition(100, 568);

    adduser.roletext.setFillColor(sf::Color::White);

    adduser.roletext.setCharacterSize(55);

    // confirmation

    adduser.confirmationtext.setFont(Calibri);

    adduser.confirmationtext.setString("If input completed click confirm");

    adduser.confirmationtext.setPosition(120, 675);

    adduser.confirmationtext.setCharacterSize(30);

    adduser.confirmationtext.setFillColor(sf::Color::Black);

    // options background

    adduser.secbackground.setTexture(secbackgroundTex);

    adduser.secbackground.setPosition(65, 140);

    adduser.secbackground.scale(0.9, 1.3);

    // main background

    adduser.background.setTexture(backgroundTex);

    adduser.background.setPosition(0, 0);

    adduser.background.setScale(0.28, 0.28);
}
void drawAddusers(AddUsers& adduser)

{
    window.draw(adduser.background);

    window.draw(adduser.secbackground);

    window.draw(adduser.usernametextbox);

    window.draw(adduser.addresstextbox);

    window.draw(adduser.emailtextbox);

    window.draw(adduser.passwordtextbox);

    window.draw(adduser.phonetextbox);

    window.draw(adduser.optionsbutton);

    window.draw(adduser.roleAdminbutton);

    window.draw(adduser.roleUserbutton);

    window.draw(adduser.usernametext);

    window.draw(adduser.addresstext);

    window.draw(adduser.emailtext);

    window.draw(adduser.passwordtext);

    window.draw(adduser.phonetext);

    window.draw(adduser.roletext);

    window.draw(adduser.confirmationtext);
}

void EditInfo_User_Functional(Edit_Info& edit_info) {
    Event event;

    window.clear();

    Set_EditInfo_User(edit_info);

    Draw_EditInfo_User(edit_info);

    editUserCredentials(currentUser_Index);

    window.display();
}

void set_manageUser(manageUser& manage_user) {
    manage_user.background.setTexture(backgroundManageUser);

    manage_user.background.setScale(0.27, 0.22);

    manage_user.semiTransparent.setTexture(showTable);

    manage_user.semiTransparent.setScale(2.95, 0.6);

    manage_user.semiTransparent.setPosition(12, 60);

    manage_user.Title.setFont(Calibri);



    manage_user.Title.setString("Manage User");

    manage_user.Title.setPosition(250, 70);

    manage_user.Title.setCharacterSize(50);

    manage_user.Title.setStyle(Text::Bold);

    manage_user.userID.setFont(Calibri);

    manage_user.userID.setString("User ID");



    manage_user.userID.setPosition(24, 190);

    manage_user.userID.setCharacterSize(39);

    manage_user.idTextBox.setTexture(textbox);

    manage_user.idTextBox.setScale(0.55, 0.6);

    manage_user.idTextBox.setPosition(150, 170);

    manage_user.removeUser.setTexture(buttonRemoveUser);

    manage_user.removeUser.setScale(0.35, 0.37);

    manage_user.removeUser.setPosition(556, 177);

    manage_user.editUser.setTexture(buttonEditUser);

    manage_user.editUser.setScale(0.6, 0.48);

    manage_user.editUser.setPosition(20, 290);

    manage_user.addUser.setTexture(buttonAddUser);

    manage_user.addUser.setScale(0.6, 0.48);

    manage_user.addUser.setPosition(400, 290);

    inputUserIDText.setFont(Calibri);
    inputUserIDText.setScale(1, 1);
    inputUserIDText.setPosition(170, 190);
    inputUserIDText.setFillColor(Color::Black);
    inputUserIDText.setString(inputUserID);
    inputUserIDText.setCharacterSize(40);
}

void draw_manageUser(manageUser manage_user) {
    window.draw(manage_user.background);

    window.draw(manage_user.semiTransparent);

    window.draw(manage_user.Title);

    window.draw(manage_user.userID);

    window.draw(manage_user.idTextBox);

    window.draw(manage_user.removeUser);

    window.draw(manage_user.editUser);

    window.draw(manage_user.addUser);
    window.draw(inputUserIDText);


}



void functioning_manageUser()
{




    window.clear();
    while (window.isOpen())
    {


        draw_manageUser(manage_user);
        window.display();
        Event event;
        while (window.pollEvent(event))
        {

            if (event.type == Event::TextEntered && isprint(event.text.unicode) && inputUserID.size() < 17)
            {


                inputUserID += static_cast<char>(event.text.unicode);
                inputUserIDText.setString(inputUserID);


            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::BackSpace)
            {
                if (!inputUserID.empty())
                {
                    inputUserID.pop_back();
                    inputUserIDText.setString(inputUserID);
                }
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2f mousepos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

                if (manage_user.removeUser.getGlobalBounds().contains(mousepos))
                {


                    if (removeUser(stoi(inputUserID)) == false)
                    {
                        RenderWindow window2(sf::VideoMode(400, 200), "Warning!");
                        while (window2.isOpen())
                        {
                            sf::Event event2;
                            while (window2.pollEvent(event2))
                            {
                                if (event2.type == sf::Event::Closed)
                                {
                                    window2.close();
                                }

                            }

                            window2.clear();
                            Text text;
                            text.setFont(Calibri);
                            text.setString("Invalid ID!");
                            text.setScale(0.5, 0.5);
                            window2.draw(text);
                            window2.display();
                        }
                    }
                    else
                        removeUser(stoi(inputUserID));
                }

            }

        }


        //if (event.type == Event::KeyReleased && event.key.code == Keyboard::BackSpace && inputUserID.size() > 0)

    }
}


void set_managePayment(managePayment& manage_payment) {
    manage_payment.background.setTexture(manage_payment.managepayment_background);

    manage_payment.background.setScale(0.27, 0.22);

    // valuefield1 -> add new payment methode

    manage_payment.valuefield1.setTexture(textbox);

    manage_payment.valuefield1.setScale(0.5, 0.4);

    manage_payment.valuefield1.setPosition(850, 250);

    // valuefield2-> delete a current payment methode

    manage_payment.valuefield2.setTexture(textbox);

    manage_payment.valuefield2.setScale(0.5, 0.4);

    manage_payment.valuefield2.setPosition(850, 450);

    // confirm button for adding payments

    manage_payment.confirm_button.setTexture(manage_payment.confirm);

    manage_payment.confirm_button.setScale(0.28, 0.24);

    manage_payment.confirm_button.setPosition(940, 330);

    // delete button for deleting payments

    manage_payment.delete_button.setTexture(manage_payment.Delete);

    manage_payment.delete_button.setScale(0.25, 0.24);

    manage_payment.delete_button.setPosition(940, 540);

    // semi transparent background to display payment methods for user

    manage_payment.backgroundview.setTexture(Signbox);

    manage_payment.backgroundview.setScale(0.43, 1.21);

    manage_payment.backgroundview.setPosition(70, 136);

    // text to add payment

    manage_payment.button1.setFont(Calibri);

    manage_payment.button1.setFillColor(Color::Black);

    manage_payment.button1.setString("Enter name of payment to add: ");

    manage_payment.button1.setPosition(860, 230);

    manage_payment.button1.setScale(0.7, 0.6);

    // text to delete payment

    manage_payment.button2.setFont(Calibri);

    manage_payment.button2.setFillColor(Color::Black);

    manage_payment.button2.setString("Enter name of payment to delete: ");

    manage_payment.button2.setPosition(860, 430);

    manage_payment.button2.setScale(0.7, 0.6);

    // setting display1 :: adding payments

    managetext1.setFont(Calibri);

    managetext1.setScale(1, 1);

    managetext1.setPosition(870, 258);

    managetext1.setFillColor(Color::Black);

    managetext1.setString(display1);

    // settind display2 :: deleting payments

    managetext2.setFont(Calibri);

    managetext2.setScale(1, 1);

    managetext2.setPosition(870, 460);

    managetext2.setFillColor(Color::Black);

    managetext2.setString(display2);
}
void Draw_managePayment(managePayment& manage_payment) {
    window.draw(manage_payment.background);

    window.draw(manage_payment.valuefield1);

    window.draw(manage_payment.valuefield2);

    window.draw(manage_payment.confirm_button);

    window.draw(manage_payment.delete_button);

    window.draw(manage_payment.backgroundview);

    window.draw(manage_payment.button1);

    window.draw(manage_payment.button2);
}

void showPaymentMehtode(vector<string> x) {
    int c = 1;
    float pos_x = 70, pos_y = 80;
    for (auto it = x.begin(); it != x.end(); ++it) {
        manage_payment.method.setFont(Calibri);
        manage_payment.method.setString("[" + to_string(c) + "] " + *it);
        manage_payment.method.setCharacterSize(24);
        manage_payment.method.setFillColor(Color::Black);
        manage_payment.method.setPosition(pos_x, pos_y);
        c++;
        pos_y += 20;
    }
}
void managePaymentMethodes()
{
    vector<string>::iterator it = paymentMethods.begin();

    string newMethode;



    while (window.isOpen()) {
        Event event;
        window.clear();

        Draw_managePayment(manage_payment);
        window.draw(managetext1);

        window.draw(managetext2);

        window.display();
        while (window.pollEvent(event)) {
            // Handle mouse click
            Vector2f mousePos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }
            // showPaymentMehtode(paymentMethods);

            if (event.type == Event::MouseButtonPressed &&
                event.mouseButton.button == Mouse::Left) {
                // Check if the mouse click is inside the first text field

                if (manage_payment.valuefield1.getGlobalBounds().contains(mousePos)) {
                    activeDisplay = true;

                }

                // Check if the mouse click is inside the second text field

                else if (manage_payment.valuefield2.getGlobalBounds().contains(mousePos)) {
                    activeDisplay = false;
                }
            }

            // Handle text input

            if (event.type == Event::TextEntered && std::isprint(event.text.unicode)) {
                if (activeDisplay) {
                    // Append the entered character to the first text display

                    if (managedisplay1.size() < 20) { // add new payment method
                        managedisplay1 += static_cast<char>(event.text.unicode);

                        managetext1.setString(managedisplay1);
                        window.draw(managetext1);
                        window.display();



                    }

                    if (manage_payment.confirm_button.getGlobalBounds().contains(mousePos)) {
                        newMethode = managedisplay1;

                        paymentMethods.push_back(newMethode);

                    }

                }

                else {
                    // Append the entered character to the second text display

                    if (managedisplay2.size() < 20) { // delete existing payment
                        managedisplay2 += static_cast<char>(event.text.unicode);

                        managetext2.setString(managedisplay2);
                        window.draw(managetext2);
                        window.display();



                    }

                    if (manage_payment.delete_button.getGlobalBounds().contains(mousePos)) {

                        for (auto it = paymentMethods.begin(); it != paymentMethods.end(); ++it) {
                            if (managedisplay2 == *it) {
                                paymentMethods.erase(it);

                            }
                        }
                    }
                    // Debugging: Print the active display status

                    cout << "Active display: " << (activeDisplay ? "managedisplay1" : "managedisplay2")
                        << endl;
                }
            }
            // Handle backspace key

            if (event.type == Event::KeyPressed &&
                event.key.code == Keyboard::BackSpace) {
                if (activeDisplay) {
                    // Delete the last character from the first text display

                    if (!managedisplay1.empty()) {
                        managedisplay1.pop_back();

                        managetext1.setString(managedisplay1);

                        newMethode = managedisplay1;
                        window.draw(managetext1);
                        window.display();
                    }

                    if (manage_payment.confirm_button.getGlobalBounds().contains(mousePos)) {
                        newMethode = managedisplay1;
                        paymentMethods.push_back(newMethode);

                    }

                }

                else {
                    // Delete the last character from the second text display

                    if (!managedisplay2.empty()) {
                        managedisplay2.pop_back();

                        managetext2.setString(managedisplay2);
                        window.draw(managetext2);
                        window.display();
                    }
                    if (manage_payment.delete_button.getGlobalBounds().contains(mousePos)) {

                        for (auto it = paymentMethods.begin(); it != paymentMethods.end(); ++it) {
                            if (managedisplay2 == *it) {
                                paymentMethods.erase(it);

                            }
                        }
                    }
                }

            }

            savePayMethodeLocally();
        }

    }
}
void set_manageMedicine(manageMedicine& manage_medicine) {
    manage_medicine.background.setTexture(backgroundManageUser);

    manage_medicine.background.setScale(0.27, 0.22);

    manage_medicine.semiTransparent.setTexture(showTable);

    manage_medicine.semiTransparent.setScale(2.95, 0.6);

    manage_medicine.semiTransparent.setPosition(12, 60);

    manage_medicine.Title.setFont(Calibri);

    manage_medicine.Title.setString("Manage Medicine");

    manage_medicine.Title.setPosition(210, 70);

    manage_medicine.Title.setCharacterSize(50);

    manage_medicine.Title.setStyle(Text::Bold);

    manage_medicine.medicineID.setFont(Calibri);

    manage_medicine.medicineID.setString("Med ID");

    manage_medicine.medicineID.setPosition(18, 190);

    manage_medicine.medicineID.setCharacterSize(39);

    manage_medicine.idTextBox.setTexture(textbox);

    manage_medicine.idTextBox.setScale(0.55, 0.6);

    manage_medicine.idTextBox.setPosition(150, 170);

    manage_medicine.removeMedicine.setTexture(buttonRemoveMedicine);

    manage_medicine.removeMedicine.setScale(0.35, 0.37);

    manage_medicine.removeMedicine.setPosition(556, 177);

    manage_medicine.editMedicine.setTexture(buttonEditMedicine);

    manage_medicine.editMedicine.setScale(0.6, 0.48);

    manage_medicine.editMedicine.setPosition(20, 290);

    manage_medicine.addMedicine.setTexture(buttonAddMedicine);

    manage_medicine.addMedicine.setScale(0.6, 0.48);

    manage_medicine.addMedicine.setPosition(400, 290);

    inputMedicineIDText.setFont(Calibri);
    inputMedicineIDText.setScale(1, 1);
    inputMedicineIDText.setPosition(170, 190);
    inputMedicineIDText.setFillColor(Color::Black);
    inputMedicineIDText.setString(inputMedicineID);
    inputMedicineIDText.setCharacterSize(40);
}




void draw_manageMedicine(manageMedicine manage_medicine)

{
    window.draw(manage_medicine.background);

    window.draw(manage_medicine.semiTransparent);

    window.draw(manage_medicine.Title);

    window.draw(manage_medicine.medicineID);

    window.draw(manage_medicine.idTextBox);

    window.draw(manage_medicine.removeMedicine);

    window.draw(manage_medicine.editMedicine);

    window.draw(manage_medicine.addMedicine);

    window.draw(inputMedicineIDText);
}

void functioning_manageMedicine()
{
    window.clear();
    while (window.isOpen())
    {
        draw_manageMedicine(manage_medicine);
        window.display();
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::TextEntered && isprint(event.text.unicode) && inputMedicineID.size() < 17)
            {

                inputMedicineID += static_cast<char>(event.text.unicode);
                inputMedicineIDText.setString(inputMedicineID);

            }
            if (event.type == Event::KeyPressed && event.key.code == Keyboard::BackSpace)
            {
                if (!inputMedicineID.empty())
                {
                    inputMedicineID.pop_back();
                    inputMedicineIDText.setString(inputMedicineID);
                }
            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2f mousepos = window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });
                if (manage_medicine.removeMedicine.getGlobalBounds().contains(mousepos))
                {
                    if (removeMedicine(stoi(inputMedicineID)) == false)
                    {
                        RenderWindow window2(sf::VideoMode(400, 200), "Warning!");
                        while (window2.isOpen())
                        {
                            sf::Event event2;
                            while (window2.pollEvent(event2))
                            {
                                if (event2.type == sf::Event::Closed)
                                {
                                    window2.close();
                                }

                            }

                            window2.clear();
                            Text text;
                            text.setFont(Calibri);
                            text.setString("Invalid ID!");
                            text.setScale(0.5, 0.5);
                            window2.draw(text);
                            window2.display();
                        }
                    }
                    else
                        removeMedicine(stoi(inputMedicineID));
                }
            }
        }
    }
}
void editUserCredentials(int index)

{
    while (window.isOpen()) {

        window.clear();

        Draw_EditInfo_User(edit_info);

        window.draw(editUtext1);
        window.draw(editUtext2);
        window.display();

        Event event;

        while (window.pollEvent(event)) {
            // Handle mouse click

            if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
            {
                window.close();
            }
            Vector2f mousePos =
                window.mapPixelToCoords({ event.mouseButton.x, event.mouseButton.y });

            if (event.type == Event::MouseButtonPressed &&
                event.mouseButton.button == Mouse::Left) {
                // Check if the mouse click is inside the first text field

                if (edit_info.valuefield1.getGlobalBounds().contains(mousePos)) {
                    editUactive = true;

                }

                // Check if the mouse click is inside the second text field

                else if (edit_info.valuefield2.getGlobalBounds().contains(mousePos)) {
                    editUactive = false;
                }
            }

            // Handle text input



            if (event.type == Event::TextEntered && std::isprint(event.text.unicode)) {
                if (editUactive) {
                    // Append the entered character to the first text display

                    if (editUdisplay1.size() < 20) {
                        editUdisplay1 += static_cast<char>(event.text.unicode);
                        editUtext1.setString(editUdisplay1);


                    }

                    if (edit_info.changeAddress.getGlobalBounds().contains(mousePos)) {
                        users[index].address = editUdisplay1;
                    }
                    window.draw(editUtext1);
                    window.display();
                }

                else {
                    // Append the entered character to the second text display

                    if (editUdisplay2.size() < 20) {
                        editUdisplay2 += static_cast<char>(event.text.unicode);

                        editUtext2.setString(editUdisplay2);

                        window.draw(editUtext2);
                        window.display();

                    }

                    if (edit_info.changePhone.getGlobalBounds().contains(mousePos)) {
                        users[index].phone = editUdisplay2;
                    }
                }
                // Debugging: Print the active display status

                cout << "Active display: " << (editUactive ? "display1" : "display2")
                    << endl;
            }
            // Handle backspace key

            if (event.type == Event::KeyPressed &&
                event.key.code == Keyboard::BackSpace) {
                if (editUactive) {
                    // Delete the last character from the first text display

                    if (!editUdisplay1.empty()) {
                        editUdisplay1.pop_back();

                        editUtext1.setString(editUdisplay1);

                        window.draw(editUtext1);
                        window.display();

                    }

                    if (edit_info.changeAddress.getGlobalBounds().contains(mousePos)) {
                        users[index].address = editUdisplay1;
                    }

                }

                else {
                    // Delete the last character from the second text display

                    if (!editUdisplay2.empty()) {
                        editUdisplay2.pop_back();

                        editUtext2.setString(editUdisplay2);

                        window.draw(editUtext2);
                        window.display();

                    }

                    if (edit_info.changePhone.getGlobalBounds().contains(mousePos)) {
                        users[index].phone = editUdisplay2;
                    }
                }
            }
        }


        saveUserDataLocally();

    }
}

void SetMedicineEdit(MedicineInfo& medicineinfo)

{
    medicineinfo.backgroundy.setTexture(backgroundsearch);

    medicineinfo.backgroundy.setScale(0.276, 0.24);

    medicineinfo.confirm.setTexture(confirm);

    medicineinfo.confirm.setScale(0.25, 0.25);

    medicineinfo.confirm.setPosition(580, 210);

    medicineinfo.quantity.setTexture(quantity);

    medicineinfo.quantity.setScale(0.25, .25);

    medicineinfo.quantity.setPosition(580, 325);

    medicineinfo.price.setTexture(price);

    medicineinfo.price.setScale(0.25, 0.25);

    medicineinfo.price.setPosition(580, 447);

    medicineinfo.valuefield3.setTexture(textbox);

    medicineinfo.valuefield3.setScale(0.5, 0.35);

    medicineinfo.valuefield3.setPosition(200, 210);

    medicineinfo.valuefield1.setTexture(textbox);

    medicineinfo.valuefield1.setScale(0.5, 0.35);

    medicineinfo.valuefield1.setPosition(200, 325);

    medicineinfo.valuefield2.setTexture(textbox);

    medicineinfo.valuefield2.setScale(0.5, 0.35);

    medicineinfo.valuefield2.setPosition(200, 445);
}
void DrawMedicineEdit(MedicineInfo medicineinfo) {
    window.draw(medicineinfo.backgroundy);

    RectangleShape backgroundRect1(Vector2f(745, 380));

    backgroundRect1.setPosition(20, 170);

    backgroundRect1.setFillColor(Color(0, 0, 0, 150));

    window.draw(backgroundRect1);

    window.draw(medicineinfo.confirm);

    window.draw(medicineinfo.price);

    window.draw(medicineinfo.quantity);

    window.draw(medicineinfo.valuefield1);

    window.draw(medicineinfo.valuefield2);

    window.draw(medicineinfo.valuefield3);
}
void MedicineEditShowFunctional(bool& medicineEdit, MedicineInfo medicineinfo) {
    Event event;

    medicineEdit = 1;

    DrawMedicineEdit(medicineinfo);

    MedicineEditShow();

    window.display();
}
// positions
void MedicineEditShow() {
    Text text;

    text.setFont(Calibri);
    text.setString("Medicine ID:");

    text.setPosition(40, 216);

    window.draw(text);

    text.setFont(Calibri);

    text.setString("Quantity : ");

    text.setPosition(40, 330);

    window.draw(text);

    text.setString("Order price:");

    text.setPosition(40, 449);

    window.draw(text);
}

void setShowAllOrders(strShowAllOrders& ShowAllOrders) {
    ShowAllOrders.background.setTexture(backgroundTex);
    ShowAllOrders.background.setPosition(0, 0);
    ShowAllOrders.background.scale(0.3, 0.29);

    ShowAllOrders.trans_back.setTexture(showTable);
    ShowAllOrders.trans_back.setOrigin(128.5, 314.5);
    ShowAllOrders.trans_back.setPosition(490, 450);
    ShowAllOrders.trans_back.scale(3.5, 0.8);

    ShowAllOrders.button1.setTexture(confirm);
    ShowAllOrders.button1.setPosition(ShowAllOrders.trans_back.getPosition().x - 145, ShowAllOrders.trans_back.getPosition().y + 150);
    ShowAllOrders.button1.scale(0.5, 0.5);
}
void drawShowAllOrders(strShowAllOrders ShowAllOrders) {
    window.draw(ShowAllOrders.background);
    window.draw(ShowAllOrders.trans_back);
    window.draw(ShowAllOrders.button1);
}

void SetEditOrderInfo(EditOrderInfo& edit) {
    // Background
    edit.Background.setTexture(BackgroundSign);
    edit.Background.setScale(0.276, 0.218);

    // SemitransparentBackground
    edit.semiTransparentBack.setTexture(Signbox);
    edit.semiTransparentBack.setScale(0.7, 1.3);
    edit.semiTransparentBack.setPosition(600, 150);

    // OrderId Text
    edit.OrderID.setFont(Calibri);
    edit.OrderID.setPosition(660, 180);
    edit.OrderID.setString("Order ID:");

    // OrderID TextBox
    edit.textboxID.setTexture(textbox);
    edit.textboxID.setScale(0.5, 0.4);
    edit.textboxID.setPosition(790, 170);

    // 1st WhiteBox
    edit.WhiteBox1.setTexture(WhiteBox);
    edit.WhiteBox1.setPosition(650, 240);
    edit.WhiteBox1.setScale(1.05, 0.8);

    // Order Details Text
    edit.OrderDetails.setFont(Calibri);
    edit.OrderDetails.setPosition(670, 245);
    edit.OrderDetails.setFillColor(Color::Black);
    edit.OrderDetails.setString("Order Details:");

    // MedicineName Text
    edit.MedicineNme.setFont(Calibri);
    edit.MedicineNme.setPosition(680, 285);
    edit.MedicineNme.setFillColor(Color::Black);
    edit.MedicineNme.setString("-Medicine Name:");

    // MedicineConcentration Text
    edit.MedicineConcentration.setFont(Calibri);
    edit.MedicineConcentration.setPosition(680, 325);
    edit.MedicineConcentration.setFillColor(Color::Black);
    edit.MedicineConcentration.setString("-Medicine Conc.:");

    // Orderdate Text
    edit.OrderDate.setFont(Calibri);
    edit.OrderDate.setPosition(680, 365);
    edit.OrderDate.setFillColor(Color::Black);
    edit.OrderDate.setString("-Order's Date:");

    // OrderState Text
    edit.OrderState.setFont(Calibri);
    edit.OrderState.setPosition(680, 405);
    edit.OrderState.setFillColor(Color::Black);
    edit.OrderState.setString("-Order's State:");

    // TotalPrice Text
    edit.TotalPrice.setFont(Calibri);
    edit.TotalPrice.setPosition(680, 445);
    edit.TotalPrice.setFillColor(Color::Black);
    edit.TotalPrice.setString("-Total Price:");

    // 2nd WhiteBox
    edit.WhiteBox2.setTexture(WhiteBox);
    edit.WhiteBox2.setPosition(650, 500);
    edit.WhiteBox2.setScale(1.05, 0.6);

    // WantToChange Text
    edit.WantChange.setFont(Calibri);
    edit.WantChange.setPosition(670, 505);
    edit.WantChange.setFillColor(Color::Black);
    edit.WantChange.setString("Want to change:");

    // 2nd OrderState Text
    edit.OrderState2.setFont(Calibri);
    edit.OrderState2.setPosition(680, 560);
    edit.OrderState2.setFillColor(Color::Black);
    edit.OrderState2.setString("-Order's State:");

    // 1st Change button
    edit.changeButton.setTexture(changeButton);
    edit.changeButton.setScale(0.25, 0.25);
    edit.changeButton.setPosition(860, 555);

    // 2nd TotalPrice Text
    edit.TotalPrice2.setFont(Calibri);
    edit.TotalPrice2.setPosition(680, 620);
    edit.TotalPrice2.setFillColor(Color::Black);
    edit.TotalPrice2.setString("-Total Price:");

    // TotalPrice Textbox
    edit.textBoxPrice.setTexture(textbox);
    edit.textBoxPrice.setPosition(830, 610);
    edit.textBoxPrice.setScale(0.2, 0.4);

    // 2nd Change button
    edit.changeButton2.setTexture(changeButton);
    edit.changeButton2.setScale(0.25, 0.25);
    edit.changeButton2.setPosition(990, 615);
}
void DrawEditOrderInfo(EditOrderInfo edit) {
    window.draw(edit.Background);
    window.draw(edit.semiTransparentBack);
    window.draw(edit.WhiteBox1);
    window.draw(edit.WhiteBox2);
    window.draw(edit.changeButton);
    window.draw(edit.changeButton2);
    window.draw(edit.MedicineConcentration);
    window.draw(edit.MedicineNme);
    window.draw(edit.OrderDate);
    window.draw(edit.OrderDetails);
    window.draw(edit.OrderID);
    window.draw(edit.OrderState);
    window.draw(edit.OrderState2);
    window.draw(edit.textboxID);
    window.draw(edit.textBoxPrice);
    window.draw(edit.TotalPrice);
    window.draw(edit.TotalPrice2);
    window.draw(edit.WantChange);
}

void logInInterface(string username, string password) {

    bool loggedIn = false;


    if (!loggedIn)
    {
        if (validateUser(username, password, currentUser))
        {
            loggedIn = true;
            //cout << "Log in success. Welcome back, " << currentUser.username << " :D\n-------------------------------------------\n";

            if (currentUser.his_role == user::User)
            {
                page_num = 2;
                window.clear();


            }
            else
            {
                window.clear();
                page_num = 3;


            }
        }
        else
        {
            window.clear();
            display1.resize(0);
            display2.resize(0);
            activeDisplay = true;
            functioningSignIn();
            window.display();
        }
    }


}

bool validateUser(string username, string password, user& currentUser) {
    int userIndex = 0;

    // Loop through the users until a user with userID = 0 is found,
    // indicating that there are no more users in our database

    while (users[userIndex].ID != 0) {
        if (users[userIndex].ID == -1) {
            userIndex++;
            continue;
        }

        if (users[userIndex].username == username &&
            users[userIndex].password == password) {
            currentUser = users[userIndex];  // Avoiding any kind of problem when
            // showing permissions based on the role
            currentUser.ID = users[userIndex].ID;
            currentUser_Index == userIndex;
            return true;
        }

        userIndex++;
    }
    return false;
}

bool isUsernameTaken(string username) {
    int i = 0;

    while (users[i].ID != 0) {
        if (users[i].ID == -1) {
            ++i;
            continue;
        }

        if (users[i].username == username) {
            return true;
        }
        ++i;
    }

    return false;
}

void showOrderReceipt(order lastOrder, string current_time) {
    // printing order date
    float y = 100, x = 20;
    Text text;
    text.setFont(Calibri);
    text.setString("order time : " + current_time);
    text.setPosition(x, y);
    window.draw(text);
    y += 30;
    text.setFont(Calibri);
    text.setString("order date : " + lastOrder.orderDate);
    text.setPosition(x, y);
    window.draw(text);
    // printing order id
    y += 30;
    text.setString("order ID : " + to_string(lastOrder.orderID));
    text.setPosition(x, y);
    window.draw(text);
    // printing medicines id
    int i = 0;
    while (lastOrder.medicine_ID[i] != 0) {
        y += 30;
        text.setString("medicine ID : " + to_string(lastOrder.medicine_ID[i]));
        text.setPosition(x, y);
        window.draw(text);
        i++;
    }
    // printing total price
    y += 30;
    text.setString("total price : " + to_string(lastOrder.totalPrice));
    text.setPosition(x, y);
    window.draw(text);
    // printing user id
    y += 30;
    text.setString("user ID : " + to_string(lastOrder.userID));
    text.setPosition(x, y);
    window.draw(text);
    // printing shipdate
    y += 30;
    text.setString("ship date : " + (lastOrder.shipDate));
    text.setPosition(x, y);
    window.draw(text);
}

void makeOrder(string medicineIDS, string quantity, string payment_method) {
    // this function might take some time to be understood ...
    // i tried my best to make it more understandable with comments
    // Good Luck
    order lastyorder = {};
    int length_medicines = medicineIDS.size();
    int length_quantity = quantity.size();
    int first_space_pos = -1, second_space_pos = -1;
    int first_space_pos_quan = -1, second_space_pos_quan = -1;
    int quantity_arr[11] = {};
    int pay_num = 2000;
    int j = 0;
    bool error_format = false;
    bool error_id = false;
    bool quantity_problem = false;
    bool pay_in_range = false;
    string current_time;
    // simple general try and catch code because this can execute some exceptions
    // dependent on user input format
    try {
        // a loop to split string input into medicine id array
        for (int i = 0; i < length_medicines + 1; i++) {
            if (medicineIDS[i] == ' ' || medicineIDS[i] == '\0') {
                first_space_pos = second_space_pos + 1;
                second_space_pos = i;
                lastyorder.medicine_ID[j] = stoi(medicineIDS.substr(
                    first_space_pos, (second_space_pos - first_space_pos)));
                j++;
            }
        }
    }
    catch (...) {
        cout << "medicine ids is in a wrong format ... this order will not be "
            "excuted"
            << endl;
        error_format = true;
    }
    j = 0;
    // simple general try and catch code because this can execute some exception
    // dependent on user input format
    try {
        // a loop to split string input into quantity array
        for (int i = 0; i < length_quantity + 1; i++) {
            if (quantity[i] == ' ' || quantity[i] == '\0') {
                first_space_pos_quan = second_space_pos_quan + 1;
                second_space_pos_quan = i;
                quantity_arr[j] =
                    stoi(quantity.substr(first_space_pos_quan, (second_space_pos_quan -
                        first_space_pos_quan)));
                j++;
            }
        }
    }
    catch (...) {
        cout << "quantities is in a wrong format ... this order will not be excuted"
            << endl;
        error_format = true;
    }
    try {
        pay_num = stoi(payment_method);
    }
    catch (...) {
        cout << "payment method is in a wrong format ... this order will not be "
            "excuted"
            << endl;
        error_format = true;
    }
    // the order will be done only if the input was in the right format
    if (error_format == false) {
        int x = 0;
        bool found = false;
        while (x != 10 && lastyorder.medicine_ID[x] != 0) {
            found = false;
            for (int k = 0; medicines[k].ID != 0; k++) {
                if (medicines[k].ID == lastyorder.medicine_ID[x]) {
                    if (medicines[k].quantity_in_stock >= quantity_arr[x]) {
                        lastyorder.totalPrice += medicines[k].price * quantity_arr[x];
                        found = true;
                    }
                    else {
                        quantity_problem = true;
                    }
                }
            }
            if (found == false) {
                error_id = true;
            }
            x++;
        }
        // check if payment method in range
        if (pay_num <= paymentMethods.size() && pay_num > 0) {
            pay_in_range = true;
        }
        // quantity will decrease only if there is no error detected
        if (quantity_problem == false && error_format == false &&
            quantity_problem == false && pay_in_range == true) {
            x = 0;
            while (x != 10 && lastyorder.medicine_ID[x] != 0) {
                for (int k = 0; medicines[k].ID != 0; k++) {
                    if (medicines[k].ID == lastyorder.medicine_ID[x]) {
                        medicines[k].quantity_in_stock -= quantity_arr[x];
                    }
                }
                x++;
            }
            lastyorder.userID = currentUser.ID;
            time_t t = time(0);  // Get time now
            tm now;
            localtime_s(&now, &t);
            lastyorder.orderDate = to_string(now.tm_year + 1900) + '-' +
                to_string(now.tm_mon + 1) + '-' +
                to_string(now.tm_mday);
            lastyorder.orderState = 0;
            lastyorder.shipDate = to_string(now.tm_year + 1900) + '-' +
                to_string(now.tm_mon + 1) + '-' +
                to_string(now.tm_mday);
            current_time = to_string(now.tm_hour) + ':' + to_string(now.tm_min) +
                ':' + to_string(now.tm_sec);
        }
        // order will be cleared if a medicine id is wrong
        if (error_id == true) {
            cout << "you entered a wrong id , your order will not be excuted" << endl;
            lastyorder = {};
            current_time = "";
        }
        // order will be cleared if we don't have the quantity needed
        if (quantity_problem) {
            cout
                << "you entered unavailable quantity , your order will not be excuted"
                << endl;
            lastyorder = {};
            current_time = "";
        }
        if (pay_in_range == false) {
            cout << "wrong payment method, your order will not be excuted" << endl;
            lastyorder = {};
            current_time = "";
        }
        // order will be saved if there isn't a single error
        if (error_id == false && error_format == false &&
            quantity_problem == false) {
            int i = 0;
            for (i; orders[i].orderID != 0; i++)
                ;
            orders[i] = lastyorder;
        }
        showOrderReceipt(lastyorder, current_time);
    }
}


void page_switcher(Header& header, SignUp& signup, SignIn& signin,
    userMenu& usermenu, adminMenu& adminmenu,
    searchMedicine& searchmedicine, showReceipt& showreceipt,
    Edit_Info& edit_info, string current_time,
    StmakeOrder makeorder, manageMedicine manage_medicine) {
    // this is a page switcher to decide which page should be displayed right now
    // don't forgot to put your function draw or you new full functional page
    // function here events such as buttons click should change page_num so the
    // page shown would be changed
    Event event;
    order lastorder = orders[1];  // this shoud be returned from makeorder when fully functional

    switch (page_num) {
    case 0:
        functioningSignUp();
        break;
    case 1:
        functioningSignIn();
        break;
    case 2:
        functioningUserMenu();
        break;
    case 3:
        functioningAdminMenu();
        break;
    case 4:

        functioningsearch();
        window.display();

        break;

    case 5:
        ShowReceiptFunctional(lastorder, show_order_receipt, showreceipt, current_time);
        break;
    case 6:
        Draw_EditInfo_Admin(edit_info);
        window.display();
        break;
    case 7:

        editUserCredentials(currentUser_Index);
        window.display();

        break;
    case 8:
        makeOrderFunctional(makeorder);
        break;
    case 9:
        drawShowAllOrders(ShowAllOrders);
        window.display();
        break;
    case 10:
        managePaymentMethodes();
        window.display();
        break;
    case 11:
        functioning_manageUser();
        window.display();
        break;
    case 12:
        functioning_manageMedicine();
        window.display();
        break;
    case 13:
        window.clear();
        DrawEditOrderInfo(editOrder);
        window.display();
        break;

    }
}