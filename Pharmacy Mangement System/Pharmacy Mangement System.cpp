//**********Libraries***********//
#include <string_view>
#include <iostream>
#include <SFML/Graphics.hpp>
#include<SFML/Window.hpp>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
#include<vector>
#include <ctime>
#include "SaveData.h"
using namespace std;
using namespace sf;
//**********Declarations***********//
extern vector<string> paymentMethods;
const int Size = 100;
int medicine_data = 0;
int user_data = 0;
int requestcounter = 0;
int chosenOption; // variable to choose with which order you want to go through after log in
bool activeDisplay = true; // Initialize active display to display1
RenderWindow window(VideoMode(1366, 768), "Your pharmacy", Style::Fullscreen);
Font Calibri;
Texture ButtonTexture, TextTexture, darkbox, textbox, headerbox, Signbox, buttonup, buttonin;
Texture ButtonEditInfo, ButtonLogOut, ButtonMakeOrder, Buttonsearch, ButtonViewPrev, backgroundUser;
Texture  ButtonManageUser, ButtonManagePay, ButtonManageOrder, ButtonManageMedicine, backgroundAdmin;
Texture byName, byCategory, searchBar, resultTable, backgroundsearch;
Texture backgroundShowReceipt, showTable, confirm;
string display1, display2;
Text displaytext1, displaytext2;

struct medicine {
	int ID;
	string name;
	string description;
	string concentration;
	bool availability;
	string category;
	float price;
	int quantity_in_stock;
	void initialize(int _id, string _name, string _desc, string _conc, bool _avail, string _category, float _price, int _quantity) {
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
	void initialize(int _id, string _name, string _pass, string _email, string _address, string _phone, role _rol) {
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
user currentUser; //Temp to keep the current user's data

struct order {
	int userID;
	string orderDate;
	int orderID;
	bool orderState = false;
	int medicine_ID[10] = {};
	float totalPrice;

	string shipDate;
	void initialize(int _id, string _orderDate, int _medicine_ID[], float _Price, string _shipDate, int order_id, bool order_state) {
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
//All GUI structures
struct Edit_Info {
	Texture edit_background, change_phone, change_address, make_admin_green, make_user_green, make_user_red, make_admin_red;
	Sprite background, changePhone, changeAddress, log_out;
	Sprite valuefield1, valuefield2, valuefield3;
	Sprite makeAdminGreen, makeUserGreen, makeAdminRed, makeUserRed;
	Text input_id;

};
Edit_Info edit_info;
struct searchMedicine {
	Sprite backgroundx;
	Sprite byName, byCategory, searchBar, resultTable;
};

struct showReceipt {
	Sprite backgroundy;
	Sprite showTable, confirm;
};
struct Header
{
	Sprite background;
	Text user, pharmacy;
};

struct SignUp {
	Sprite background1, background2;  // background 1 is for fields ..... background 2 is for sign in instead option   
	Sprite buttonup, buttonin;
	Sprite valuefield1, valuefield2, valuefield3, valuefield4, valuefield5; //username, phone num, location, role, new password
	Text user, location, number, role, forgot, password;

};

struct SignIn {
	Sprite background1, background2;    // background 1 is for fields ..... background 2 is for sign in instead option 
	Sprite buttonup, buttonin;
	Sprite valuefield1, valuefield2;
	Text user, password, alreadyhave;
};
struct userMenu {
	Sprite buttonEditInfo, buttonLogOut, buttonMakeOrder, buttonsearch, buttonViewPrev;
	Sprite background;
};
userMenu usermenu;

struct adminMenu {
	Sprite buttonViewPrev, buttonsearch, buttonManageUser, buttonManagePay, buttonManageOrder, buttonManageMedicine, buttonLogOut, buttonEditInfo, buttonMakeorder;
	Sprite background;
};
adminMenu adminmenu;

//********Function Declares***********//
void manage_orders(order orders[Size]);
void dataForTestPurposes();
bool isUsernameTaken(string username);
void signUp();
bool validateUser(string username, string password, user& currentUser);
void logInInterface();
void userPermissions();
void adminPermissions();
void editUserCredentials(int index);
bool searchForMedicineByName();
void searchForMedicineByCategory();
void makeOrder(string medicineIDS);
void showOrderReceipt(order lastOrder,string current_time);
void makeRequest(string _username, string _medicineName, int _amountReq);
void showAllPreviousOrders();
void addUser();
void updateUser();
void removeUser();
void logOut();
void managePaymentMethodes();
void showPaymentMehtode(vector<string> x);
void logInInterface(string username, string password);
//**********GUI FUNCTIONS DECLARATION***********//
void TextureAFonts();

void DrawUserMenu(userMenu usermenu);
void SetUserMenu(userMenu& usermenu);

void DrawAdminMenu(adminMenu adminmenu);
void SetAdminMenu(adminMenu& adminmenu);

void DrawSearch(searchMedicine searchmedicine);
void SetSearch(searchMedicine& searchmedicine);

void DrawShowReceipt(showReceipt showreceipt);
void SetShowReceipt(showReceipt& showreceipt);

void DrawSignUp(SignUp signup);
void SetSignUp(SignUp& signup);
void functioningSignUp();

void DrawSignIn(SignIn signin);
void SetSignIn(SignIn& signin);
void functioningSignIn();

void Draw_EditInfo_User(Edit_Info& edit_info);
void Draw_EditInfo_Admin(Edit_Info& edit_info);

void showOrderReceipt(order lastOrder, string current_time);
void ShowReceiptFunctional(order lastOrder, bool& show_order_receipt, showReceipt showreceipt, string current_time);

void page_switcher(Header& header, SignUp& signup, SignIn& signin, userMenu& usermenu,
	adminMenu& adminmenu, searchMedicine& searchmedicine, showReceipt& showreceipt, Edit_Info& edit_info,
	string current_time);

void Set_EditInfo_User(Edit_Info& edit_info);
void Set_EditInfo_Admin(Edit_Info& edit_info);


bool sign_up;
bool show_order_receipt = 0;
int page_num = 5;
int main()
{
	dataForTestPurposes();
	saveAllDataToArr();

	TextureAFonts();
	sign_up = true;
	Header header;
	SignUp signup;
	SignIn signin;
	//userMenu usermenu;
	//adminMenu adminmenu;
	searchMedicine searchmedicine;
	showReceipt showreceipt;
	Edit_Info edit_info;
	//background
	Texture backgroundTexture;
	backgroundTexture.loadFromFile("Assets/pharmacy2.jpg");
	Sprite background;
	background.setTexture(backgroundTexture);
	background.setScale(0.276, 0.218);

	//setting up headers and sign up/in 
	//SetHeader(header);
	SetSignUp(signup);
	SetSignIn(signin);
	SetUserMenu(usermenu);
	SetAdminMenu(adminmenu);
	SetSearch(searchmedicine);
	SetShowReceipt(showreceipt);
	Set_EditInfo_Admin(edit_info);
	Set_EditInfo_User(edit_info);
	dataForTestPurposes();

	//window display
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			/*if (event.type == Event::Closed)
			{
				window.close();
			}*/
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
			{
				window.close();
			}

			if (event.type == Event::MouseButtonPressed) {
				// Check if left mouse button is pressed
				if (show_order_receipt) {
					if (event.mouseButton.button == Mouse::Left) {
						// Get the current mouse position
						Vector2i mousePosition = Mouse::getPosition(window);
						// Check if the mouse position intersects with confirm button
						if (showreceipt.confirm.getGlobalBounds().contains(static_cast<Vector2f>(mousePosition))) {
							//action performed to get to main menu
							show_order_receipt = 0;
							if (currentUser.his_role == user::User)
							{
								window.clear();
								page_num = 2;
							}
							else
							{
								window.clear();
								page_num = 3;
							}
						}
					}
				}
			}

			if (event.type == Event::MouseButtonPressed) {
				// Check if left mouse button is pressed
				if (event.mouseButton.button == Mouse::Left) {
					// Get the current mouse position
					Vector2i mousePosition = Mouse::getPosition(window);
					// Check if the mouse position intersects with signup button
					if (sign_up)
					{
						if (signup.buttonin.getGlobalBounds().contains(static_cast<Vector2f>(mousePosition))) {
							//action performed for sign in
							sign_up = false;
						}
					}
					else
					{
						if (signin.buttonup.getGlobalBounds().contains(static_cast<Vector2f>(mousePosition)) and !sign_up) {
							sign_up = true;
						}
					}
				}
			}
			window.clear();
			/*if (sign_up) {
				DrawSignUp(signup);
				window.draw(displaytext);
			}
			else {
				DrawSignIn(signin);

			}*/
			page_switcher(header, signup, signin, usermenu, adminmenu, searchmedicine,
				showreceipt, edit_info, "12:00");
		}

	}
	}
//**********Functions***********//

void dataForTestPurposes() {

	//*******************Medicine data****************************
	medicines[0].initialize(1, "Paracetamol", "Pain reliever and fever reducer", "500 mg", true, "Analgesic", 5.99, 100);
	medicines[1].initialize(2, "Lisinopril", "Used to treat high blood pressure", "10 mg", true, "Antihypertensive", 10.49, 50);
	medicines[2].initialize(3, "Omeprazole", "Treats heartburn, stomach ulcers, and gastroesophageal reflux disease (GERD)", "20 mg", true, "Gastrointestinal", 7.25, 80);
	medicines[3].initialize(4, "Atorvastatin", "Lowers high cholesterol and triglycerides", "20 mg", true, "Lipid-lowering agent", 15.75, 30);
	medicines[4].initialize(5, "Metformin", "Treats type 2 diabetes", "500 mg", true, "Antidiabetic", 8.99, 60);
	medicines[5].initialize(6, "Amoxicillin", "Antibiotic used to treat bacterial infections", "250 mg", true, "Antibiotic", 6.50, 0);
	medicines[6].initialize(7, "Alprazolam", "Treats anxiety and panic disorders", "0.25 mg", true, "Anxiolytic", 12.99, 40);
	medicines[7].initialize(8, "Ibuprofen", "Nonsteroidal anti-inflammatory drug (NSAID)", "200 mg", true, "Analgesic", 4.75, 200);
	medicines[8].initialize(9, "Cetirizine", "Antihistamine used for allergy relief", "10 mg", true, "Antihistamine", 9.25, 0);
	medicines[9].initialize(10, "Ranitidine", "Reduces stomach acid production to treat heartburn and ulcers", "150 mg", true, "Gastrointestinal", 6.99, 90);

	//--------------------------------------------------------------------------------------------------------------------------------------------------------

	//*******************User data****************************
	users[0].initialize(1, "Naruto", "NotNaruto", "narutouzumaki@example.com", "123 Main St, Cityville", "+1234567890", user::User);
	users[1].initialize(2, "Madara", "password2", "nadarauchiha@example.com", "456 Elm St, Townsville", "+1987654321", user::User);
	users[2].initialize(3, "Cillian", "Cillianpass", "callianmurphy@example.com", "789 Oak St, Villageton", "+1122334455", user::Admin);
	users[3].initialize(4, "Aras", "passBulut", "arasbulut@example.com", "987 Pine St, Hamletville", "+9988776655", user::User);
	users[4].initialize(5, "Sung", "jinwoo", "sungjinwoo@example.com", "654 Birch St, Countryside", "+1122334455", user::User);
	users[5].initialize(6, "Iman", "imangadzhi", "imangadzhi@example.com", "321 Maple St, Suburbia", "+9988776655", user::User);
	users[6].initialize(7, "Ali", "AliAli", "alimuhammadali.smith@example.com", "111 Cedar St, Ruralville", "+1122334455", user::Admin);

	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	int medicine1[] = { 1, 2, 3, 0 };
	orders[0].initialize(1, "2024-03-27", medicine1, 500.0, "2024-03-27", 1, false);
	int medicine2[] = { 4, 5, 0 };
	orders[1].initialize(2, "2024-03-28", medicine2, 300.0, "2024-03-28", 2, true);
	int medicine3[] = { 9, 4,5,7 ,0 };
	orders[2].initialize(3, "2024-03-29", medicine3, 3000.0, "2024-03-29", 3, true);


}


void signUp() {

	int id = user_data + 1; // Next available ID

	newUser.ID = id;

	do {
		cout << "Enter your username: ";
		cin >> newUser.username;

		if (isUsernameTaken(newUser.username))
			cout << "A user with that username already exists. Please enter a different username: ";


	} while (isUsernameTaken(newUser.username)); //Checks if username is already in our database or no.

	cout << "Enter your password: ";
	cin >> newUser.password;
	cout << "Enter your e-mail: ";
	cin >> newUser.email;
	cout << "Enter your address: ";
	cin.ignore(1, '\n');
	getline(cin, newUser.address);
	cout << "Enter your phone: ";
	cin >> newUser.phone;

	int roleChoice;
	do {
		cout << "Pick your role\n1-User\n2-Admin\n";
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
	} while (roleChoice != 1 && roleChoice != 2); // Loop until a valid role choice is made

	users[id - 1] = newUser; // Save the new user data into our users array

	cout << "Congratulations! Your account has been successfully created.\n";

	saveOneUserDataLocally();

	user_data++;										 // Increment user_data to keep track of the total number of users
}


void logInInterface()
{
	bool loggedIn = false;

	while (!loggedIn)
	{


		cout << "Enter your username: ";
		cin >> currentUser.username;
		cout << "Enter your password: ";
		cin >> currentUser.password;

		if (validateUser(currentUser.username, currentUser.password, currentUser)) {

			loggedIn = true;
			cout << "Log in success. Welcome back, " << currentUser.username << " :D\n-------------------------------------------\n";


			if (currentUser.his_role == user::User)
			{
				userPermissions();
			}
			else
			{
				adminPermissions();
			}

			cin >> chosenOption;
		}


		else
		{
			cout << "Invalid credentials. The username or password you entered is incorrect. Please try again.\n";

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

void editUserCredentials(int index)
{
	cout << "What are you willing to change ?\n";
	cout << "1- Phone Number\n";
	cout << "2- Address\n";
	int choice;
	do {

		cin >> choice;
		if (choice == 1) {
			cout << "Enter your new phone number: ";
			cin >> users[index].phone;

		}
		else if (choice == 2) {
			cout << "Enter your new adress: ";
			cin.ignore(1, '\n');
			getline(cin, users[index].address);
		}
		else {
			cout << "Invalid choice. Please enter 1 for Phone Number or 2 for Address.\n";
		}
	} while (choice != 1 && choice != 2); // Loop until a valid choice is made

	saveUserDataLocally();


}

bool searchForMedicineByName() {
	string name;
	cout << "Enter the medicine name\n";
	cin >> name;
	int x = name.size();
	if (name[0] >= 'a' && name[0] <= 'z') {
		name[0] -= 32;
	}
	int i = 0;
	bool found = 0;
	bool instock = 1;
	while (medicines[i].ID != 0)
	{
		string_view sv(medicines[i].name.c_str(), x);
		if (name == sv) {
			cout << medicines[i].ID << " -- " << medicines[i].name << " -- " << medicines[i].availability << " -- " << medicines[i].category << " -- " << medicines[i].price << " -- " << medicines[i].quantity_in_stock << '\n';
			if (medicines[i].quantity_in_stock <= 0) {
				instock = 0;
			}
			found = 1;
		}
		i++;
	}
	if (instock && found) {
		return 1;
	}
	else {
		int amountrequested;
		cout << "Enter the amount you need:\n";
		cin >> amountrequested;
		makeRequest(currentUser.username, name, amountrequested);
	}
}

void searchForMedicineByCategory() {
	string category;
	cin >> category;
	bool found = false;
	for (int i = 0; i < Size; i++) {
		if (category == medicines[i].category) {
			cout << medicines[i].name << " -- " << medicines[i].ID << " -- " << medicines[i].category << '\n';
			found = true;
		}
	}
	if (found == false) {
		cout << "there is no medicine meet this category\n";
		int ifwant;
		string medicineName;
		int amountrequested;
		cout << "Want to make a request?choose(any number for yes/ 0 for no)" << endl;
		cin >> ifwant;
		if (ifwant != 0)
		{
			cout << "Enter medicine name:\n";
			cin >> medicineName;
			cout << "Enter the amount you need:\n";
			cin >> amountrequested;
			makeRequest(currentUser.username, medicineName, amountrequested);
		}
	}
}

// Convert date string to integers
void parseDateString(const std::string& dateString, int& year, int& month, int& day) {
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


void makeRequest(string _username, string _medicineName, int _amountReq)
{
	_username = currentUser.username;
	int response;
	cout << "Confirm the request? choose (1 for yes / 0 for no): " << endl;
	cin >> response;
	//no cases because it will be handeled in gui
	requestcounter++;
	if (response == 1) {
		for (int i = 0; i < Size; i++)
		{
			if (_username == users[i].username)
			{
				currentUser.ID = users[i].ID;
			}
		}
		for (int j = 0; j < 15; j++)
		{
			if (requests[j].userID == 0)
			{
				requests[j].userID = currentUser.ID;
				requests[j].medicineName = _medicineName;
				requests[j].amountNeeded = _amountReq;
				break;
			}
		}
	}
}

void showAllPreviousOrders() {
	for (int i = 0; i < Size; i++) {   // checking for the current user ID to be able to get his/her orders using ID
		if (currentUser.username == users[i].username) {
			currentUser.ID == users[i].ID;
			break;
		}
	}

	cout << "Your previous orders: \n";
	cout << "---------------------\n";
	int num_order = 1;   // to display list of numbered orders
	bool found_orders = false;  // to check if there were no orders regesitered for this user
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

			//getting medicine name from medicine ID
			for (int i = 0; i < Size; i++) {
				if (medicines[i].ID == currentID) {
					cout << "Name of the medicine: " << medicines[i].name << "\n";
					cout << "concentraion of the medicine" << medicines[i].concentration << "\n";
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
void showPaymentMehtode(vector<string> x)
{
	int c = 1;
	for (auto it = x.begin(); it != x.end(); ++it)
	{
		cout << "[" << c << "] " << *it << endl;
		c++;
	}
}
void managePaymentMethodes()
{
	vector<string>::iterator it = paymentMethods.begin();
	char chooseP;

	int m = 0;
	while (true)
	{
		if (m > 0)
		{
			cout << "invalid input try again from the list given : \n";
		}
		cout << "Edit payment methodes : " << "\n";
		cout << "[1] show existed payment methodes\n";
		cout << "[2] add new payment methode      \n";
		cout << "[3] delete payment methode       \n";
		cout << "choose from the above : "; cin >> chooseP;
		m++;
		if (chooseP >= '1' && chooseP <= '3')
		{
			break;
		}
		system("cls");
	}


	switch (chooseP)
	{
	case '1':
	{
		system("cls");
		cout << "=== payment methodes available now are === \n";
		showPaymentMehtode(paymentMethods);
		break;
	}
	case '2':
	{
		system("cls");
		string newMethode;
		cout << "=== Adding new Payment methode ===\n";
		cout << "insert method's name: ";
		cin.ignore(1, '\n');
		getline(cin, newMethode, '\n');
		cout << "methode now available are: \n";
		paymentMethods.push_back(newMethode);
		showPaymentMehtode(paymentMethods);
		savePayMethodeLocally();
		break;
	}
	case '3':
	{
		system("cls");
		int chooseD;
		cout << "\n=== delete payment methode ===\n";
		showPaymentMehtode(paymentMethods);
		cout << "\nchoose methode to delete from the given list: "; cin >> chooseD;
		*it = chooseD - 1;
		paymentMethods.erase(it, it + 1);
		cout << "methodes available now is : \n\n";
		showPaymentMehtode(paymentMethods);
		savePayMethodeLocally();
	}

	}
}

void addUser()
{
	int id = user_data + 1;
	newUser.ID = id;

	cout << "Please provide the following information for the new user:\n";
	do {
		cout << "Username: ";
		cin >> newUser.username;

		if (isUsernameTaken(newUser.username))

			cout << "A user with that username already exists. Please enter a different username: ";


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

	user_data++;										 // Increment user_data to keep track of the total number of users

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
					cout << "The new username is the same as the current one. Please enter a different username.\n";

				}
				else if (isUsernameTaken(newUsername)) {
					cout << "A user with that username already exists. Please enter a different username.\n";

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

		cout << "Would you like to make any additional edits? Choose between y/n : ";
		cin >> choice;

	} while (choice == 'y' || choice == 'Y');

	cout << "User updated successfully!\n";

	saveOneUserDataLocally();
}

void removeUser()
{
	int userID;
	bool userFound = false;

	while (!userFound) {
		cout << "Enter the ID of the user you are willing to remove: ";
		cin >> userID;

		if (userID <= 0) {
			cout << "Invalid ID.\n";
			continue;
		}


		int i = 0;
		while (users[i].ID != 0) {
			if (users[i].ID == userID) {
				userFound = true;
				users[i].ID = -1;
				cout << "User with ID: " << userID << " has been removed.\n";
				saveUserDataLocally();
				break;
			}
			++i;


		}

		if (!userFound) {
			cout << "User not found.\n";
		}
	}
}

void trackorder(order orders[])
{
	bool orderfound = false;
	int orderid = -1;
	cout << "enter id : ";
	cin >> orderid;
	for (size_t i = 0; i < 3; i++)
	{
		if (orderid == orders[i].orderID)
		{

			if (orders[i].orderState == 0)
			{
				cout << "OrderNotDelivered" << endl;

			}
			else
			{
				cout << "OrderDelivered" << endl;
			}
			orderfound = true;
		}

	}
	if (orderfound != true)
	{
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

	if (found) { // display the order first
		for (int j = 0; j < 10; j++) {
			if (orders[indx].medicine_ID[j] == 0) {
				break;
			}
			for (int i = 0; i < Size; i++) {

				if (medicines[i].ID == orders[indx].medicine_ID[j]) {
					cout << "Name of the medicine: " << medicines[i].name << "\n";
					cout << "concentraion of the medicine" << medicines[i].concentration << "\n";
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

		cout << "Would you like to make any additional edits? Choose between y/n : ";
		cin >> answer;

	} while (answer == 'y' || answer == 'Y');
	cout << "Order updated successfully!\n";
}

void logOut()
{
	logInInterface(); //Basically, just open the log in interface again if you are willing to log out 
}
void TextureAFonts()
{
	//inserting images and fonts
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

	// edit info images
	edit_info.edit_background.loadFromFile("Assets/pharmacy-with-nurse-in-counter-drugstore-cartoon-character-vector (1).jpg");
	edit_info.change_address.loadFromFile("Assets/change_add.png");
	edit_info.change_phone.loadFromFile("Assets/change_phone.png");
	edit_info.make_admin_green.loadFromFile("Assets/green_make_admin.png");
	edit_info.make_admin_red.loadFromFile("Assets/red_make_admin.png");
	edit_info.make_user_green.loadFromFile("Assets/green_make_user.png");
	edit_info.make_user_red.loadFromFile("Assets/red_make_user.png");
}
void DrawHeader(Header header)
{
	window.draw(header.background);
	window.draw(header.user);
	window.draw(header.pharmacy);
}
void SetHeader(Header& header)
{
	// background of header
	//header.background.setTexture(headerbox);
	//header.background.setOrigin(900 / 2, 650 / 2);
	//header.background.setPosition(190, 0);
	//61804header.background.setScale(2.5, 0.45);
	//text for header
	header.pharmacy.setFont(Calibri);
	//header.pharmacy.setString("Your Pharmacy..Wherever you are");
	//header.pharmacy.setPosition(300, 10);
	//header.pharmacy.setScale(1, 1);
	//header.pharmacy.setCharacterSize(60);
}

void DrawSignUp(SignUp signup)
{
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
}
void SetSignUp(SignUp& signup)
{
	//background 1 located behind value fields
	signup.background1.setTexture(Signbox);
	signup.background1.setPosition(650, 156.2);
	signup.background1.setScale(0.7, 1.25);
	// valuefield1 -> username
	signup.valuefield1.setTexture(textbox);
	signup.valuefield1.setScale(0.6, 0.4);
	signup.valuefield1.setPosition(800, 175);
	//valuefield2-> phone num
	signup.valuefield2.setTexture(textbox);
	signup.valuefield2.setScale(0.6, 0.4);
	signup.valuefield2.setPosition(800, 275);
	//valuefield3->location
	signup.valuefield3.setTexture(textbox);
	signup.valuefield3.setScale(0.6, 0.4);
	signup.valuefield3.setPosition(800, 375);
	//valuefield4->role
	signup.valuefield4.setTexture(textbox);
	signup.valuefield4.setScale(0.6, 0.4);
	signup.valuefield4.setPosition(800, 475);
	//valuefield5-> new password
	signup.valuefield5.setTexture(textbox);
	signup.valuefield5.setScale(0.6, 0.4);
	signup.valuefield5.setPosition(800, 575);

	//button for sign up
	signup.buttonup.setTexture(buttonup);
	signup.buttonup.setScale(0.28, 0.24);
	signup.buttonup.setPosition(890, 665);

	//texts for info needed in sign up
	signup.user.setFont(Calibri);
	signup.user.setString("Username:");
	signup.user.setPosition(680, 190);
	signup.user.setScale(0.8, 0.7);
	signup.number.setFont(Calibri);
	signup.number.setString("Phone num:");
	signup.number.setPosition(680, 290);
	signup.number.setScale(0.8, 0.7);
	signup.location.setFont(Calibri);
	signup.location.setString("Location:");
	signup.location.setPosition(680, 390);
	signup.location.setScale(0.8, 0.7);
	signup.role.setFont(Calibri);
	signup.role.setString("Role:");
	signup.role.setPosition(680, 490);
	signup.role.setScale(0.8, 0.7);
	signup.password.setFont(Calibri);
	signup.password.setString("password:");
	signup.password.setPosition(680, 590);
	signup.password.setScale(0.8, 0.7);

	//log in button
	signup.buttonin.setTexture(buttonin);
	signup.buttonin.setScale(0.28, 0.24);
	signup.buttonin.setPosition(240, 645);

	// text to log in instead if you already have an account
	signup.forgot.setFont(Calibri);
	signup.forgot.setString("Already have an account?");
	signup.forgot.setScale(1, 1);
	signup.forgot.setPosition(170, 560);

	//background 2-> located behind log in button
	signup.background2.setTexture(Signbox);
	signup.background2.setPosition(66, 525);
	signup.background2.setScale(0.55, 0.45);
}

void DrawSignIn(SignIn signin) {
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
	//background 1 located behind value fields
	signin.background1.setTexture(Signbox);
	signin.background1.setPosition(650, 365.5);
	signin.background1.setScale(0.7, 0.8);


	// valuefield1 -> username
	signin.valuefield1.setTexture(textbox);
	signin.valuefield1.setScale(0.6, 0.4);
	signin.valuefield1.setPosition(800, 430);
	//valuefield2-> phone num
	signin.valuefield2.setTexture(textbox);
	signin.valuefield2.setScale(0.6, 0.4);
	signin.valuefield2.setPosition(800, 550);

	//texts for value fields
	signin.user.setFont(Calibri);
	signin.user.setString("Username:");
	signin.user.setPosition(680, 450);
	signin.user.setScale(0.7, 0.6);
	signin.password.setFont(Calibri);
	signin.password.setString("Password:");
	signin.password.setPosition(680, 570);
	signin.password.setScale(0.7, 0.6);
	//background 2-> located behind sign up button
	signin.background2.setTexture(Signbox);
	signin.background2.setPosition(66, 525);
	signin.background2.setScale(0.55, 0.45);

	//text for sign up 
	signin.alreadyhave.setFont(Calibri);
	signin.alreadyhave.setString("Don't have an account?");
	signin.alreadyhave.setPosition(170, 560);
	signin.alreadyhave.setScale(1, 1);
	/// sign up button
	signin.buttonup.setTexture(buttonup);
	signin.buttonup.setScale(0.28, 0.24);
	signin.buttonup.setPosition(235, 645);

	//log in button
	signin.buttonin.setTexture(buttonin);
	signin.buttonin.setScale(0.28, 0.24);
	signin.buttonin.setPosition(890, 630);
}
void functioningSignIn() {

	SignIn signin;
	while (window.isOpen()) {

		//setting display1 :: username

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


		TextureAFonts();

		SetSignIn(signin);



		Texture backgroundTexture;
		backgroundTexture.loadFromFile("Assets/pharmacy2.jpg");
		Sprite background;
		background.setTexture(backgroundTexture);
		background.setScale(0.276, 0.218);


		window.clear();
		DrawSignIn(signin);
		window.draw(displaytext1);
		window.draw(displaytext2);
		window.display();

		Event event;

		while (window.pollEvent(event))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
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

				else if (signin.valuefield2.getGlobalBounds().contains(mousePos)) {

					activeDisplay = false;
				}

				if (signin.buttonin.getGlobalBounds().contains(mousePos))
				{
					logInInterface(display1, display2);
				}
			}

			// Handle text input
			 // Handle text input
			if (event.type == Event::TextEntered && std::isprint(event.text.unicode)) {
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
		}
	}
}

void SetUserMenu(userMenu& usermenu) {
	//setting background 
	usermenu.background.setTexture(backgroundUser);
	usermenu.background.setScale(0.276, 0.24);

	//setting Edit button
	usermenu.buttonEditInfo.setTexture(ButtonEditInfo);
	usermenu.buttonEditInfo.setPosition(100, 170);
	usermenu.buttonEditInfo.setScale(0.7, 0.7);

	//setting LogOut button
	usermenu.buttonLogOut.setTexture(ButtonLogOut);
	usermenu.buttonLogOut.setPosition(1100, 680);
	usermenu.buttonLogOut.setScale(0.3, 0.3);

	//setting Make Order Button
	usermenu.buttonMakeOrder.setTexture(ButtonMakeOrder);
	usermenu.buttonMakeOrder.setPosition(100, 420);
	usermenu.buttonMakeOrder.setScale(0.7, 0.7);

	//setting Search button
	usermenu.buttonsearch.setTexture(Buttonsearch);
	usermenu.buttonsearch.setPosition(860, 170);
	usermenu.buttonsearch.setScale(0.7, 0.7);

	//setting View Previous Orders button
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

void SetAdminMenu(adminMenu& adminmenu) {
	//setting background
	adminmenu.background.setTexture(backgroundAdmin);
	adminmenu.background.setScale(0.276, 0.24);

	//setting Edit Info button
	adminmenu.buttonEditInfo.setTexture(ButtonEditInfo);
	adminmenu.buttonEditInfo.setScale(0.5, 0.5);
	adminmenu.buttonEditInfo.setPosition(550, 140);

	//setting Log Out button
	adminmenu.buttonLogOut.setTexture(ButtonLogOut);
	adminmenu.buttonLogOut.setScale(0.28, 0.28);
	adminmenu.buttonLogOut.setPosition(1100, 680);

	//setting Make Order button
	adminmenu.buttonMakeorder.setTexture(ButtonMakeOrder);
	adminmenu.buttonMakeorder.setScale(0.5, 0.5);
	adminmenu.buttonMakeorder.setPosition(550, 270);

	//setting Manage Medicine button
	adminmenu.buttonManageMedicine.setTexture(ButtonManageMedicine);
	adminmenu.buttonManageMedicine.setScale(0.5, 0.5);
	adminmenu.buttonManageMedicine.setPosition(550, 410);

	//setting Manage Order button
	adminmenu.buttonManageOrder.setTexture(ButtonManageOrder);
	adminmenu.buttonManageOrder.setScale(0.5, 0.5);
	adminmenu.buttonManageOrder.setPosition(550, 540);

	//setting Manage Pay button
	adminmenu.buttonManagePay.setTexture(ButtonManagePay);
	adminmenu.buttonManagePay.setScale(0.5, 0.5);
	adminmenu.buttonManagePay.setPosition(950, 140);

	//setting Manage User button
	adminmenu.buttonManageUser.setTexture(ButtonManageUser);
	adminmenu.buttonManageUser.setScale(0.5, 0.5);
	adminmenu.buttonManageUser.setPosition(950, 270);

	//setting Search button
	adminmenu.buttonsearch.setTexture(Buttonsearch);
	adminmenu.buttonsearch.setScale(0.5, 0.5);
	adminmenu.buttonsearch.setPosition(950, 410);

	//setting View Previous button
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

void DrawSearch(searchMedicine searchmedicine) {
	window.draw(searchmedicine.backgroundx);
	window.draw(searchmedicine.searchBar);
	window.draw(searchmedicine.resultTable);
	window.draw(searchmedicine.byCategory);
	window.draw(searchmedicine.byName);
}
void SetSearch(searchMedicine& searchmedicine) {
	//setting background
	searchmedicine.backgroundx.setTexture(backgroundsearch);
	searchmedicine.backgroundx.setScale(0.276, 0.24);
	//setting byname
	searchmedicine.byName.setTexture(byName);
	searchmedicine.byName.setScale(0.332, 0.42);
	searchmedicine.byName.setPosition(184, 181);
	//setting bycategory
	searchmedicine.byCategory.setTexture(byCategory);
	searchmedicine.byCategory.setScale(0.332, 0.42);
	searchmedicine.byCategory.setPosition(0.3, 181);
	//setting searchbar
	searchmedicine.searchBar.setTexture(searchBar);
	searchmedicine.searchBar.setScale(0.69, 0.55);
	searchmedicine.searchBar.setPosition(366.4, 181.5);
	//setting resultTable
	searchmedicine.resultTable.setTexture(resultTable);
	searchmedicine.resultTable.setScale(0.71, 0.49);
	searchmedicine.resultTable.setPosition(0, 254);
}
void SetShowReceipt(showReceipt& showreceipt)
{
	//setting showtable
	showreceipt.showTable.setTexture(showTable);
	showreceipt.showTable.setScale(1.28, 0.8);
	showreceipt.showTable.setPosition(12, 90);
	//setting background
	showreceipt.backgroundy.setTexture(backgroundShowReceipt);
	showreceipt.backgroundy.setScale(0.276, 0.24);
	//setting confirm buttom
	showreceipt.confirm.setTexture(confirm);
	showreceipt.confirm.setScale(0.4, 0.4);
	showreceipt.confirm.setPosition(67, 610);
}
void DrawShowReceipt(showReceipt showreceipt) {
	window.draw(showreceipt.backgroundy);
	window.draw(showreceipt.showTable);
	window.draw(showreceipt.confirm);

}
void ShowReceiptFunctional(order lastOrder, bool& show_order_receipt, showReceipt showreceipt, string current_time) {
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

	//valuefield2 for change phone number input
	edit_info.valuefield2.setTexture(textbox);
	edit_info.valuefield2.setScale(0.6, 0.4);
	edit_info.valuefield2.setPosition(150, 440);

}
void Draw_EditInfo_User(Edit_Info& edit_info) {
	window.draw(edit_info.background);
	window.draw(edit_info.changeAddress);
	window.draw(edit_info.changePhone);
	window.draw(edit_info.valuefield1);
	window.draw(edit_info.valuefield2);
}

void Set_EditInfo_Admin(Edit_Info& edit_info) {
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

	//valuefield2 for change phone number input
	edit_info.valuefield2.setTexture(textbox);
	edit_info.valuefield2.setScale(0.6, 0.4);
	edit_info.valuefield2.setPosition(150, 440);

	//valuefield3 to input the username of the user that i want to edit 
	edit_info.valuefield3.setTexture(textbox);
	edit_info.valuefield3.setScale(0.6, 0.4);
	edit_info.valuefield3.setPosition(400, 150);

	// make user red and green buttons
	edit_info.makeUserGreen.setTexture(edit_info.make_user_green);
	edit_info.makeUserGreen.setScale(0.28, 0.24);
	edit_info.makeUserGreen.setPosition(200, 555);

	edit_info.makeUserRed.setTexture(edit_info.make_user_red);
	edit_info.makeUserRed.setScale(0.28, 0.24);
	edit_info.makeUserRed.setPosition(200, 555);

	//make admin red and green buttons
	edit_info.makeAdminGreen.setTexture(edit_info.make_admin_green);
	edit_info.makeAdminGreen.setScale(0.28, 0.24);
	edit_info.makeAdminGreen.setPosition(500, 555);

	edit_info.makeAdminRed.setTexture(edit_info.make_admin_red);
	edit_info.makeAdminRed.setScale(0.28, 0.24);
	edit_info.makeAdminRed.setPosition(500, 555);

	edit_info.input_id.setFont(Calibri);
	edit_info.input_id.setString("Enter Username: ");
	edit_info.input_id.setScale(1, 1);
	edit_info.input_id.setFillColor(Color::Black);
	edit_info.input_id.setPosition(190, 155);

}
void Draw_EditInfo_Admin(Edit_Info& edit_info) {
	window.draw(edit_info.background);
	window.draw(edit_info.changeAddress);
	window.draw(edit_info.changePhone);
	window.draw(edit_info.valuefield1);
	window.draw(edit_info.valuefield2);
	window.draw(edit_info.valuefield3);
	window.draw(edit_info.input_id);


	int userIndex = 0;

	// Loop through the users until a user with userID = 0 is found,
	// indicating that there are no more users in our database
	while (users[userIndex].ID != 0)
	{
		if (users[userIndex].username == currentUser.username) {
			break;
		}
		userIndex++;
	}

	//draws buttons with respect to the role of that user

	if (currentUser.his_role == user::User) {
		window.draw(edit_info.makeUserGreen);
		window.draw(edit_info.makeAdminRed);

		Vector2i mousePosition = Mouse::getPosition(window);

		if (edit_info.makeAdminRed.getGlobalBounds().contains(static_cast<Vector2f>(mousePosition))) {
			currentUser.his_role = user::Admin;
			users[userIndex].his_role = user::Admin;

		}
	}
	if (currentUser.his_role == user::Admin) {
		window.draw(edit_info.makeAdminGreen);
		window.draw(edit_info.makeUserRed);

		Vector2i mousePosition = Mouse::getPosition(window);

		if (edit_info.makeUserRed.getGlobalBounds().contains(static_cast<Vector2f>(mousePosition))) {
			currentUser.his_role = user::User;
			users[userIndex].his_role = user::User;
		}
	}

}

void page_switcher(Header& header, SignUp& signup, SignIn& signin, userMenu& usermenu,
	adminMenu& adminmenu, searchMedicine& searchmedicine, showReceipt& showreceipt, Edit_Info& edit_info, string current_time)
{
	// this is a page switcher to decide which page should be displayed right now
	// don't forgot to put your function draw or you new full functional page function here 
	// events such as buttons click should change page_num so the page shown would be changed
	order lastorder = orders[1];//this shoud be returned from makeorder when fully functional
	switch (page_num) {
	case 0:
		DrawSignIn(signin);
		window.display();
		break;
	case 1:
		functioningSignIn();
		break;
	case 2:
		DrawUserMenu(usermenu);
		window.display();
		break;
	case 3:
		DrawAdminMenu(adminmenu);
		window.display();
		break;
	case 4:
		DrawSearch(searchmedicine);
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
		Draw_EditInfo_User(edit_info);
		window.display();
		break;
	case 8:

		break;
	}
}
void logInInterface(string username, string password)
{
	TextureAFonts();
	SetAdminMenu(adminmenu);
	SetUserMenu(usermenu);

	bool loggedIn = false;

	while (window.isOpen())
	{
		if (!loggedIn)
		{
			if (validateUser(username, password, currentUser))
			{
				loggedIn = true;
				//cout << "Log in success. Welcome back, " << currentUser.username << " :D\n-------------------------------------------\n";

				if (currentUser.his_role == user::User)
				{
					window.clear();
					DrawUserMenu(usermenu);
					window.display();
				}
				else
				{
					window.clear();
					DrawAdminMenu(adminmenu);
					window.display();
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
}

bool validateUser(string username, string password, user& currentUser)
{
	int userIndex = 0;

	// Loop through the users until a user with userID = 0 is found,
	// indicating that there are no more users in our database

	while (users[userIndex].ID != 0)
	{
		if (users[userIndex].ID == -1)
		{
			userIndex++;
			continue;
		}

		if (users[userIndex].username == username && users[userIndex].password == password)
		{
			currentUser = users[userIndex]; //Avoiding any kind of problem when showing permissions based on the role 
			currentUser.ID = users[userIndex].ID;
			return true;
		}

		userIndex++;
	}
	return false;
}

bool isUsernameTaken(string username) {

	int i = 0;

	while (users[i].ID != 0) {
		if (users[i].ID == -1)
		{
			++i;
			continue;
		}

		if (users[i].username == username)
		{
			return true;
		}
		++i;
	}

	return false;
}

void showOrderReceipt(order lastOrder, string current_time) {
	//printing order date
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
	//printing order id
	y += 30;
	text.setString("order ID : " + to_string(lastOrder.orderID));
	text.setPosition(x, y);
	window.draw(text);
	//printing medicines id
	int i = 0;
	while (lastOrder.medicine_ID[i] != 0) {
		y += 30;
		text.setString("medicine ID : " + to_string(lastOrder.medicine_ID[i]));
		text.setPosition(x, y);
		window.draw(text);
		i++;
	}
	//printing total price
	y += 30;
	text.setString("total price : " + to_string(lastOrder.totalPrice));
	text.setPosition(x, y);
	window.draw(text);
	//printing user id
	y += 30;
	text.setString("user ID : " + to_string(lastOrder.userID));
	text.setPosition(x, y);
	window.draw(text);
	//printing shipdate
	y += 30;
	text.setString("ship date : " + (lastOrder.shipDate));
	text.setPosition(x, y);
	window.draw(text);

}
void makeOrder(string medicineIDS) {
	// this function might take some time to be understood ... 
	// i tried my best to make it more understandable with comments
	//Good Luck
	order lastyorder = {};
	int length = medicineIDS.size();
	int first_space_pos = -1, second_space_pos = -1;
	int j = 0;
	bool error_format = false;
	bool error_id = false;
	string current_time;
	// simple general try and catch code because this can execute some exception dependent on user input format
	try {
		// a loop to split string input into medicine id array
		for (int i = 0; i < length + 1; i++) {
			if (medicineIDS[i] == ' ' || medicineIDS[i] == '\0') {
				first_space_pos = second_space_pos + 1;
				second_space_pos = i;
				lastyorder.medicine_ID[j] = stoi(medicineIDS.substr(first_space_pos, (second_space_pos - first_space_pos)));
				j++;
			}
		}
	}
	catch (...) {
		cout << "your input is in a wrong format ... this order will not be excuted" << endl;
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
					lastyorder.totalPrice += medicines[k].price;
					found = true;
				}
			}
			if (found == false) {
				error_id = true;
			}
			x++;
		}

		lastyorder.userID = currentUser.ID;
		time_t t = time(0); // Get time now
		tm now;
		localtime_s(&now, &t);
		lastyorder.orderDate = to_string(now.tm_year + 1900) + '-' + to_string(now.tm_mon + 1) + '-' + to_string(now.tm_mday);
		lastyorder.orderState = 0;
		lastyorder.shipDate = to_string(now.tm_year + 1900) + '-' + to_string(now.tm_mon + 1) + '-' + to_string(now.tm_mday);
		current_time = to_string(now.tm_hour) + ':' + to_string(now.tm_min) + ':' + to_string(now.tm_sec);

	}
	//order will only be saved or excuted if all ids are right
	if (error_id == true) {
		cout << "you entered a wrong id , your order will not be excuted" << endl;
		lastyorder = {};
		current_time = "";
	}
	if (error_id == false && error_format == false) {
		int i = 0;
		for (i; orders[i].orderID != 0; i++);
		orders[i] = lastyorder;
	}
	showOrderReceipt(lastyorder, current_time);
}