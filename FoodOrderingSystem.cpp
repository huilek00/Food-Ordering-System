#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

const int SIZE = 10; // Constant global variable for the size of food menu array

// Function prototypes
void readFile(string [], float [], int [], float []); 
void displayMenu_manager(string[], float[], int[], float[]); 
int getUserType(); 
int updateMenu_or_getSales(); 
int getFoodCode(); 
int askWhatToUpdate(); 
void updateFoodName(string [], int); 
void updatePrice(float [], int); 
void updateStock(int [], int); 
void updatePreparationTime(float [], int); 
int askUpdateMenuAgain(); 
void displayMenu_customer(string [], float []); 
int getNumFood_order(); 
int getQuantity_order(); 
bool checkStock(int, int [], int [], int, int); 
void displayReceipt(string [], int [], float [], int, float, float, float, float, float); 
int getPaymentMethod(); 
void getAddress(); 
void getCreditCardInfo();
int verifyCardInfo(); 
void displaySales(string [], float [], int [], float, int); 
void writeFile(string [], float [], int [], float, int); 

int main()
{
	string arrFood[SIZE];			// Parallel array to store food names
	float arrPrice[SIZE];			// Parallel array to store food price
	int arrStock[SIZE];  			// Parallel array to store food stock
	float arrTime[SIZE];			// Parallel array to store food preparation time (minute)
	
	int userType = 1,			// Type of user (manager/customer)
	 	choice_manager,			// Choice of manager (update menu / get total sales)
	 	whatToUpdate = 1,		// Things to be updated on the menu (food/price/stock/time)
	 	foodCode = 0,			// Food code that represents each of the food
	 	isUpdateAgain = 0;		// Choice of manager whether to make more changes on menu

	int count = 0,				// Counter variable
		size_order = 0,			// Array size determined by customer while ordering food (number of types of food)
	    foodCode_order = 0,		// Food code entered by customer while choosing food
	    qty_order = 0;			// Quantity of each type of food ordered by a customer
	
	string *arrFood_order = nullptr;	// To dynamically allocate an array to store the food ordered by customer
	float *arrPrice_order = nullptr; 	// To dynamically allocate an array to store the total price of each food 
	int *arrQty_order = nullptr;		// To dynamically allocate an array to store the quantity of each food
		
	int arrSalesFood[SIZE] = {}; 		// Array to store the sales (quantity) of each type of food
	
	bool stockEnough = true;			// Boolean variable to check the stock of food
	
	const float DELIVERY_TAX = 7.00,	// Constant variable for the delivery tax (RM)
				SERVICE_TAX_PCT = 0.06, // Constant variable for the percentage of service tax (%)
				DELIVERY_TIME = 15.00;  // Constant variable for the delivery time (minute)
				
	float prepareTime = 0,				// Total preparation time for one type of food
	      totalPrepareTime = 0,			// Accumulator to store preparation time for all food per order 
		  totalTime = 0,				// Sum of preparation time and delivery time per order
		  totalFoodPrice = 0,			// Accumulator to store the total food price payable by a customer
	      totalSales = 0, 				// Accumulator to store the total sales(RM) (only food price) per day
	      serviceTax = 0,				// Service tax per order (not include delivery tax)
		  paymentPerOrder = 0;			// Total payment of customer per order
	      
	int paymentMethod,					// Choice of payment method of customer
		isVerify,					    // Choice of customer to verity the credit card info
		numCustomer = 0;				// Accumulator to store total number of customers per day
	
	// Read the menu from a file
	readFile(arrFood, arrPrice, arrStock, arrTime);
	
	// After reading the menu, this program starts with asking for the user type 
	// Do-while loop that continues asking for the user type until manager choose to get the sales of the day
	do{
	// Function call to get the type of user ( 1 = manager, 2 = customer)
	userType = getUserType();
	
	// If the user type is manager
	if(userType == 1)
	{
		// Function call to get the choice of manager (1 = update menu, 2 = get the sales)
		choice_manager = updateMenu_or_getSales();
		
		// If manager chooses to update the menu
		if(choice_manager == 1) 
		{
			// Display the menu for manager which contains food, price, stock and preparation time
			displayMenu_manager(arrFood, arrPrice, arrStock, arrTime);
			
			// Do-while loop to update the menu, the loop keeps running if manager still wants to update menu
			do{
				// Function call to ask manager to choose a food to update on the menu
				cout << "\nWhich food do you choose to update on the menu?\n" ;
				foodCode = getFoodCode(); 
				
				// Function call to ask manager what to update (food/price/stock/preparation time)
				whatToUpdate = askWhatToUpdate();
				
				// Use switch statement to call the function to update a food on the menu
				// according to the choice of manager
				switch(whatToUpdate)
				{
					// Function call to update food name
					case 1:	updateFoodName(arrFood, foodCode); 
							break;
					// Function call to update price
					case 2:	updatePrice(arrPrice, foodCode); 
							break;
					// Function call to update food stock
					case 3: updateStock(arrStock, foodCode);
	 						break;
	 				// Function call to update food preparation time
					case 4: updatePreparationTime(arrTime, foodCode);
	 						break;
	 				// Function call to update all of the above (food, price, stock & preparation time)
	 				case 5: updateFoodName(arrFood, foodCode);
	 						updatePrice(arrPrice, foodCode);
	 						updateStock(arrStock, foodCode);
							updatePreparationTime(arrTime, foodCode);
							break;
					default: cout << "Error!\n";
				} 
				// Display the updated version of menu after updating one type of food
				displayMenu_manager(arrFood, arrPrice, arrStock, arrTime);
				cout << "Table shown above is the updated version of the menu.\n";
				
				// Ask manager whether want to make more changes on the menu (1 = Yes, 0 = No)
				isUpdateAgain = askUpdateMenuAgain(); 
					
			} while(isUpdateAgain != 0); // The loop will stop once manager chooses 'No' to update menu again 
 		}
 		// If manager chooses to get the sales 
		else if(choice_manager == 2) 
		{
			// Display the total sales of a day
			displaySales(arrFood, arrPrice, arrSalesFood, totalSales, numCustomer);
			
			// Write the total sales into a file
			writeFile(arrFood, arrPrice, arrSalesFood, totalSales, numCustomer);
			
			cout << "The total sales are written to Sales.txt file.\n";
		}
	}
	
	// If the user type is customer
	else if(userType == 2)
	{
		// Display the menu for customer which contains food and price
		displayMenu_customer(arrFood, arrPrice);
		
		// Function call to get the number of different types of food customer want to order
		size_order = getNumFood_order();
		
		// Dynamically allocate the arrays large enough to hold the FOOD NAME, TOTAL PRICE PER TYPE OF FOOD
		// and QUANTITY PER TYPE OF FOOD with the array size (number of different types of food)
		// determined by customer.  
		arrFood_order = new string[size_order];
		arrPrice_order = new float[size_order]; 
		arrQty_order = new int[size_order];	
		
		// For loop for the customer to order food. The loop will terminates when the counter variable 
		// reaches the total number of different types of food chosen by customer.  
		for(count = 0; count < size_order; count++)
		{
			cout << "\n**Food Type " << count + 1 << "**"; 
			// Do-while loop to let customer reorder the food if the food is out of stock	
			do{
			// Ask customer to choose the food by getting the food code (1-10)
			cout << "\nWhich food do you choose to order on the menu?\n" ;
			foodCode = getFoodCode();
			
			// Allocate the memory of food name from the array of food (arrFood) into the array that 
			// contains all food names ordered by customer (arrFood_order).
			// The food code is from 1-10, while the subscript of all of the arrays starts with 0, 
			// so foodCode_order has to subtract 1 in the subscript of array.
			arrFood_order[count] = arrFood[foodCode - 1];
			
			// Get quantity of the food chosen by the customer
			qty_order = getQuantity_order();
			
			// Allocate the memory of food quantity into the array contains quantity of each food 
			// ordered by customer (arrQty_order).
			arrQty_order[count] = qty_order;
			
			// Check whether the stock of food is enough
			stockEnough = checkStock(foodCode, arrStock, arrQty_order, size_order, count);
			
			} while(stockEnough != true); // The loop continues to run if the stock is not enough
			
			// Update the array of food stock by subtracting the quantity of each food ordered by customer
			arrStock[foodCode - 1] -= qty_order;
			
			// Accumulate the total sales(quantity) of each food into an array that contains total sales 
			// for each type of food (arrSalesFood)
			arrSalesFood[foodCode - 1] += arrQty_order[count]; 
			   
			// Calculate the total price of a type of food by multiplying price of food with its quantity 
			// and allocate the memory into the array that store the price per type of food (arrPrice_order)
			arrPrice_order[count] = arrPrice[foodCode - 1] * arrQty_order[count];
			
			// Calculate the total preparation time of a type of food by multiplying the time with food quantity
			prepareTime = arrTime[foodCode - 1] * arrQty_order[count]; 
			
			// Accumulate the total price of food per order
			totalFoodPrice += arrPrice_order[count];

			// Accumulate the total food preparation time per order
			totalPrepareTime += prepareTime;
		} 
		// Calculate the service tax by multiplying the total food price with service tax percentage
		serviceTax = totalFoodPrice * SERVICE_TAX_PCT;
		
		// Calculate the total payment per order
		paymentPerOrder = serviceTax + DELIVERY_TAX + totalFoodPrice;
		
		// Accumulate the total sales of food(RM) (not including service tax and delivery tax)  
		totalSales += totalFoodPrice;
		
		// Calculate the estimated total time required to prepare and deliver the food
		totalTime = totalPrepareTime + DELIVERY_TIME;  
		
		// Display the receipt
		displayReceipt(arrFood_order, arrQty_order, arrPrice_order, count, totalFoodPrice, 
		               DELIVERY_TAX, serviceTax, paymentPerOrder, totalTime);

		// Get the delivery address from customer
		cout << "\nPlease enter the following information for your delivery address.\n";
		getAddress();
	
		// Get the payment method from customer (1 = cash, 2 = credit/debit card)			
		paymentMethod = getPaymentMethod();
		
		// If customer chooses to pay by cash, display the message
		if(paymentMethod == 1)
		cout << "\nYou will receive your food within the estimated delivery time.\n" 
			 << "Please prepare your cash. Thank you.\n";
			 
		// If customer choose to pay by credit/debit card	 
		else if(paymentMethod == 2)
		{
			// Do-while loop that continues to run while card info has not been verified by customer
			do{
			// Get the credit card info 
			getCreditCardInfo();
			
			// Get the billing address from customer
			cout << "\nPlease enter the following information for your billing address.\n";
			getAddress();
			
			// Ask for verification of card info
			isVerify = verifyCardInfo();
			
			} while(isVerify != 1); // Loop continues to run while card info has not been verified.
			
			// Display the message once payment is done.
			cout << "\nYour payment has been made.\n"
			     << "You will receive your food within the estimated delivery time. Thank you.";
		}
		cout << "\n------------------------------------------------------------------\n";
		
		// Adding the number of customer by one each time when ordering has done.
		numCustomer++;
		
		// Make total food price and total food preparation time back to 0.
		totalFoodPrice = 0;
		totalPrepareTime = 0;
		
		// Free dynamically allocated memory, so that the arrays can be stored with new elements with 
		// new size when another customer make order. 
		delete[] arrFood_order;
		delete[] arrPrice_order;
		delete[] arrQty_order;
		
		// Makes arrFood_order, arrPrice_order and arrQty_order a null pointer
		arrFood_order = nullptr;
		arrPrice_order = nullptr;
		arrQty_order = nullptr;
	}
	} while(choice_manager != 2); // The loop of food ordering system will terminate when manager choose 
								  // to get the sales of the day.
	
	return 0;
}

//**************************************************************************************************
// The readFile function reads the content of the menu file into 4 parallel arrays.                *
// The Menu.txt file consists of 4 columns.                                                        *
// 1st column: food name, 2nd column: price, 3rd column: stock, 4th column: food preparation time  *
//**************************************************************************************************
void readFile(string food[], float price[], int stock[], float time[])
{
	// Counter variable
	int count = 0;
		
	// File stream object
	ifstream inFile;
	
	// Open the file
	inFile.open("FoodMenu.txt");
	
	if(inFile)
	{
		//while(count < size && inFile) 
		// Read the contents of file into the food, price, stock and preparation time parallel arrays 
		while(!inFile.eof())
		{
			getline(inFile, food[count], '\t');
			inFile >> price[count];
			inFile >> stock[count];
			inFile >> time[count]; 
			inFile.ignore();
			count++;
		}
		// Close the file
		inFile.close();	
	}
	else
	// Display the message if the file cannot be opened
	cout << "Error opening the file.\n"; 
}

//*******************************************************************************************
// The getUserType function asks the user to determine whether they are manager or customer *
// and return their choice.  																*
//*******************************************************************************************
int getUserType()
{
	int choice;
	
	cout << "----------------------------------------------------------------------------------------------------\n"
	     << "				 Welcome to Tasty's Restaurant\n"
	     << "----------------------------------------------------------------------------------------------------\n"
	     << "\nAre you a restaurant manager or customer?\n"
	     << "1 - Restaurant Manager\n"
	     << "2 - Customer\n";
	// Do-while loop to input and validate the choice of user
    do{
	cout << "Please enter 1/2 according to your choice: ";
    cin >> choice;
    if (choice != 1 && choice != 2)
    cout << endl << choice << " is not a valid option. Try again." << endl;
    } while(choice != 1 && choice != 2);
    
    return choice;
}

//****************************************************************************************
// The updateMenu_or_getSales function gets the choice of manager whether to update menu *
// or get the sales of the day and return the choice.									 *
//****************************************************************************************
int updateMenu_or_getSales()
{
	int choice;
	
	cout << "\nWould you like to update the menu or get the total sales?\n"
		 << "1 - Update menu (food / price / stock / food preparation time)\n"
		 << "2 - Get total sales of today.\n";
	// Do-while loop to input and validate the choice entered by user
	do{	
	cout << "Enter 1/2 according to your choice: ";
	cin >> choice;
	if(choice != 1 && choice != 2)
    cout << endl << choice << " is not a valid option. Try again." << endl;
	} while(choice != 1 && choice != 2);
	
	return choice;
}

//**********************************************************************************************************
// The displayMenu_manager function display the menu that contains food, price, stock and preparation time *
// when the manager wants to update the menu.															   *
//**********************************************************************************************************
void displayMenu_manager(string food[], float price[], int stock[], float time[])
{
	cout << "\n-------------------------------------------------------------------------------------\n"
	     << "No." << setw(21) << "Food" << "\tPrice(RM)" << "\tStock\t\tPreparation Time(min)\n"
	     << "-------------------------------------------------------------------------------------\n";
	     
	// For loop to print each of the food, price, stock and preparation time from the arrays in 4 columns
	for(int count = 0; count < SIZE; count++)
	{
		cout << fixed << showpoint << setprecision(2);
		cout << setw(2) << count + 1 << ". " << setw(20) << food[count] << "\t" 
		     << price[count] << "\t\t" << stock[count] << "\t\t" << time[count] << endl;
	}
	cout << "-------------------------------------------------------------------------------------\n";
}

//**********************************************************************************************
// The getFoodCode_update function asks the manager to choose a food to update by entering the *
// food code according to the menu.                                                            *
//**********************************************************************************************
int getFoodCode()
{
	int choice;
	     
	// Do-while loop to input and validate the choice entered by user
	do{
	cout << "Please enter the code of food (1-" << SIZE << ") according to the menu: ";
	cin >> choice;
	if(choice < 1 || choice > SIZE)
	cout << endl << choice << " is not a valid option. Try again." << endl; 
	} while(choice < 1 || choice > SIZE);
	return choice;
}

//***********************************************************************************************
// The askWhatToUpdate function asks the manager to choose the things to be updated for a food  *
// and return their choice.                                                                     *
//***********************************************************************************************
int askWhatToUpdate()
{
	int choice;
	cout << "\nWhat do you want to update on this food?\n"
	     << "1 - change food name\n"
		 << "2 - change food price\n"
		 << "3 - update food stock\n"
		 << "4 - change preparation time of food\n"
		 << "5 - change all of the above (name, price, stock and preparation time) of a food\n";
		 
	// Do-while loop to input and validate the choice of manager
	do{	
	cout << "Enter 1/2/3/4/5 according to your choice: ";
	cin >> choice;
	if(choice < 1 || choice > 5)
    cout << endl << choice << " is not a valid option. Try again." << endl;
    } while(choice < 1 || choice > 5);
    
    return choice;
}

//******************************************************************************
// The updateFoodName function asks the manager to enter the name of new food. *
//******************************************************************************
void updateFoodName(string food[], int foodCode)
{
	string newFood;	 // Name of new food
	
	// Get the name of new food item
	cout << "Enter the name of new food item: ";
	cin.ignore();
	getline(cin, newFood);
	
	// The name of new food will be stored as an element in the food array with the subscript (foodCode - 1) 
	// because the subscript of array starts with 0 while the foodCode starts with 1.
	food[foodCode-1] = newFood; 
}

//******************************************************************************
// The updatePrice function asks the manager to enter the new price of a food. *
//******************************************************************************
void updatePrice(float price[], int foodCode)
{
	float newPrice;	// Updated price of food
	
	cout << "Enter the new price of the food item: RM";
	// Do-while loop to input and validate the updated price of food.
	do{
	cin >> newPrice;
	if(newPrice < 0)
	cout << "\nThe price you entered is invalid. Please enter a positive value: RM";	
	}while(newPrice < 0);
	
	// The updated price will be stored as an element in the price array with the subscript (foodCode - 1) 
	// because the subscript of array starts with 0 while the foodCode starts with 1
	price[foodCode-1] = newPrice;
}

//***************************************************************************
// The updateStock function asks the manager to enter the stock of a food.  *
//***************************************************************************     
void updateStock(int stock[], int foodCode)
{
	int newStock;	//  Updated stock of food
	
	cout << "Enter the updated stock of the food item: ";
	// Do-while loop to input and validate the updated stock of food.
	do{
	cin >> newStock;
	if(newStock < 0)
	cout << "\nThe data you entered is invalid. Please enter a positive value: ";
	} while(newStock < 0);
	
	// The updated price will be stored as an element in the stock array with the subscript (foodCode - 1) 
	// because the subscript of array starts with 0 while the foodCode starts with 1.
	stock[foodCode-1] = newStock;
}

//********************************************************************************************************
// The updatePreparationTime function asks the manager to enter the updated preparation time of a food.  *
//********************************************************************************************************  
void updatePreparationTime(float time[], int foodCode)
{
	float newTime;	// Updated preparation time of food
	
	cout << "Enter the new preparation time of the food item (minute): ";
	// Do-while loop to input and validate the updated preparation time of food. 
	do{
	cin >> newTime;
	if(newTime < 0)
	cout << "\nThe time you entered is invalid. Please enter a positive value: ";
	} while(newTime < 0);
	
	// The updated preparation time will be stored as an element in the time array with the subscript 
	// (foodCode - 1) because the subscript of array starts with 0 while the foodCode starts with 1.
	time[foodCode-1] = newTime;
}

//***************************************************************************************************
// The askUpdateMenuAgain function asks manager whether want to make more changes on the menu or not*
// and returns the choice.																			*
//***************************************************************************************************
int askUpdateMenuAgain()
{
	int choice;
	
	cout << "\nWould you like to make more changes on the menu?\n"
		     << "1 - Yes\n"
			 << "0 - No\n";
	// Do-while loop to input and validate the choice of manager
	do{	
	cout << "Enter 1/0 according to your choice: ";
	cin >> choice;
	if(choice != 1 && choice != 0)
    cout << endl << choice << " is not a valid option. Try again." << endl;
	} while(choice != 1 && choice != 0);
	
	return choice;
}

//*************************************************************************************************
// The displayMenu_customer function display the menu for customer which contains food and price. *
//*************************************************************************************************
void displayMenu_customer(string food[], float price[])
{
	cout << "\n--------------------------------------------------\n"
	     << "No." << setw(21) << "Food" << "\tPrice(RM)" << endl
	     << "--------------------------------------------------\n";
	     
	// For loop to display the food and prices from the arrays into two columns
	for(int count = 0; count < SIZE; count++)
	{
		cout << fixed << showpoint << setprecision(2);
		cout << setw(2) << count + 1 << ". " << setw(20) << food[count] << "\t" << price[count] << endl;
	}
	cout << "--------------------------------------------------\n";
}

//*************************************************************************************************************
// The getNumFood_order function asks the customer to enter the number of types of food he/she wants to order *
//and returns the number.                                                                                     *
//*************************************************************************************************************
int getNumFood_order()
{
	int num;  //number of types of food
	
	cout << "How many different types of food you want to order? ";
	// Do-while loop to input and validate the number of types of food
	do{
	cin >> num;
	if(num <= 0)
	cout << "Invalid value. Please enter a positive number: ";
	}while(num <= 0);
	
	return num;
}

//*********************************************************************************************************
// The getQuantity_order function asks the customer to enter the quantity of a food he/she wants to order *
// and returns the quantity.                                                                              *
//*********************************************************************************************************
int getQuantity_order()
{
	int qty; // Quantity of a food
	
	cout << "Enter quantity of this food you want: ";
	// Do-while loop to input and validate the quantity of a food entered by user
	do{
	cin >> qty;
	if(qty <= 0)
	cout << endl << "Invalid value. Please enter a positive number: ";
	}while(qty <= 0);
	
	return qty;
}

//**********************************************************************************************
// The checkStock function checks the stock of each food and returns the boolean acceptOrder.  *
// A message will be displayed to let the customer know how many stock is left.                *
//**********************************************************************************************
bool checkStock(int foodCode, int stock[], int qty[], int size_order, int count)
{
	bool acceptOrder = true; // Boolean variable to determine whether the order of customer can be accepted
	
	// The current stock of food is found by subtracting the initial food stock with the food quantity
	// entered by user.
	// If the value founded is negative, it shows that the stock of food is not enough.
	if((stock[foodCode - 1] - qty[count]) < 0)
	{
		cout << "\nThe stock of this food is not enough. "
			 << "Only " << stock[foodCode - 1] << " left in stock.\n" 
			 << "Please reorder the food.";
		acceptOrder = false; // order cannot be accepted if the stock is not enough
	}
	return acceptOrder;
}

//*******************************************************************************************
// The displayReceipt function displays the receipt when customer has ordered all the foods *
// Total food price, service tax, delivery tax, total payment and estimated delivery time   *
// will be shown in the receipt.													        *
//*******************************************************************************************
void displayReceipt(string food[], int qty[], float price[], int count, float totalFoodPrice,
                    float deliveryCharge, float serviceCharge, float totalPayment, float totalTime)

{ 
	cout << "\n-----------------------------------------------------------\n"
	     << "No." << setw(21) << "Food" << "\tQuantity\tPrice(RM)" << endl
	     << "-----------------------------------------------------------\n";
	     
	// For loop to display the array of food, quantity and total price of each type of food
	// into 3 columns
	for(int i = 0; i < count; i++)
	{
		cout << fixed << showpoint << setprecision(2);
		cout << setw(2) << i + 1 << ". " << setw(20) << food[i] << "\t" << qty[i]
		     << "\t\t" << price[i] << endl;
	}
	cout << "-----------------------------------------------------------\n"
		 << "Total food price       : RM" << totalFoodPrice << endl
		 << "Service tax            : RM" << serviceCharge << endl
		 << "Delivery tax           : RM" << deliveryCharge << endl
		 << "Total payment          : RM" << totalPayment << endl 
	     << "Estimated delivery time: " << totalTime << " mins" << endl;
	     
}

//****************************************************************************
// The getAddress function asks the user to enter the information of address *
//****************************************************************************
void getAddress()
{
	int houseNum, 
	    postcode;    
	string street, 
	       city, 
		   state;
		   
	// Ask user to enter the house number / lot number / floor
	cout << "House Number / Lot Number / Floor: ";
	// Do-while loop to input and validate the house number
	do{
	cin >> houseNum;
	if(houseNum <= 0)
	cout << endl << "Invalid number. Please enter a positive number: ";
	}while(houseNum <= 0);
	
	// Ask user to enter the street / district name
	cout << "Street / District: ";
	cin.ignore();
	getline(cin, street);
	
	// Ask user to enter the postcode
	cout << "Postcode: ";
	// Do-while loop to input and validate the postcode entered by user
	do{
	cin >> postcode;
	if(postcode < 0 || postcode < 10000 || postcode >= 100000)
	cout << endl << "Invalid postcode. Please enter a positive number with 5 digits: ";
	}while(postcode < 0 || postcode < 10000 || postcode >= 100000);
	
	// Ask user to enter city name
	cout << "City: ";
	cin.ignore();
	getline(cin, city);
	
	// Ask user to enter state
	cout << "State: ";
	cin.ignore();
	getline(cin, state);
}

//***************************************************************************************************
// The getPaymentMethod function asks the user to choose the payment method, and return the choice. *
//***************************************************************************************************
int getPaymentMethod()
{
	int choice;  // choice of payment method 
	
	cout << "\nPlease choose your payment method.\n"
		 << "1 - Cash\n"
		 << "2 - Credit / Debit card\n";
	// Do-while loop to input and validate the choice entered by user
	do{	
	cout << "Enter 1/2 according to your choice: ";
	cin >> choice;
	if(choice != 1 && choice != 2)
    cout << endl << choice << " is not a valid option. Try again." << endl;
	} while(choice != 1 && choice != 2);
	
	return choice;
}

//**********************************************************************************************
// The getCreditCardInfo function asks the user to enter the information of credit/debit card. *
//**********************************************************************************************
void getCreditCardInfo()
{
	string cardNum,  // Card number
		   cvc, 	 // Card verification code
		   name;	 // Name of cardholder
	int expMonth, 	 // Expiration month
	    expYear;	 // Expiration year
	    
	cout << "Please enter the following card information.\n";
	
	// Do-while loop to input and validate the card number.
	do{
	cout << "Card Number: ";
	cin >> cardNum;
	if(cardNum.length() != 16)
	cout << "\nInvalid card number. Please enter again.\n";
	} while(cardNum.length() != 16);
	
	cout << "Card Expiration Date: ";
	// Do-while loop to input and validate expiration month.
	do{
	cout << "Month (MM): ";
	cin >> expMonth;
	if(expMonth < 1 || expMonth > 12)
	cout << "\nInvalid month. Please enter again.\n";
	} while(expMonth < 1 || expMonth > 12);
	
	// Do-while loop to inout and validate expiration year.
	// (A credit card expires 3-4 years after they're issued)
	do{
	cout << "Year (YYYY): ";
	cin >> expYear;
	if(expYear < 2021 || expYear > 2025)
	cout << "\nInvalid year. Please enter again.\n";
	} while(expYear < 2021 || expYear > 2025);
	
	// Do-while loop to input and validate the CVC / CVV number
	do{
	cout << "CVC / CVV: ";
	cin >> cvc;
	if(cvc.length() != 3)
	cout << "\nInvalid CVC/CVV number. Please enter again.\n";
	} while(cvc.length() != 3);
	
	// Get the cardholder name
	cout << "Cardholder Name: ";
	cin.ignore();
	getline(cin, name);
}

//****************************************************************************************************
// The verifyCardInfo function asks the user whether to verify the information of credit/debit card, *
// and return the choice.																			 *
//****************************************************************************************************
int verifyCardInfo()
{
	int choice;
	
	cout << "\nWould you like to verify your card information?\n"
		     << "1 - Yes\n"
			 << "0 - No\n";
	// Do-while loop to input and validate the user's choice
	do{	
	cout << "Enter 1/0 according to your choice: ";
	cin >> choice;
	if(choice != 1 && choice != 0)
    cout << endl << choice << " is not a valid option. Try again." << endl;
	} while(choice != 1 && choice != 0);
	
	return choice;
}

//**********************************************************************************************
// The displaySales function displays the price, sales(quantity) of each food, total sales(RM),*
// total number of customer	and the most popular dish at the end of the program.   			   *
//**********************************************************************************************
void displaySales(string food[], float price[], int salesFood[], float totalSales, int numCustomer)
{
	int highest = salesFood[0]; // highest number of sales
	string highestSaleFood;		// food with highest sales

	// For loop to find the most popular dish from the salesFood array and food array 
	for(int i = 0; i < SIZE; i++)
	{
		if(salesFood[i] > highest)
		{
			highest = salesFood[i];
			highestSaleFood = food[i];	// Assign highestSaleFood with the element
										// of highest sales in food array 
		}
	}
	cout << "\n----------------------------------------------------------------------------\n"
	     << "No." << setw(21) << "Food" << "\tPrice(RM)" << "\tSales(Quantity)\n"
	     << "----------------------------------------------------------------------------\n";
	cout << fixed << showpoint << setprecision(2);
	
	// For loop to display the array of food, price and salesFood in 3 columns
	for(int i = 0; i < SIZE; i++)
	{
		cout << setw(2) << i + 1 << ". " << setw(20) << food[i] << "\t" 
		     << price[i] << "\t\t" << salesFood[i] << endl;
	}
	cout << "----------------------------------------------------------------------------\n";
	cout << "Total sales (excluding service tax and delivery tax): RM" << totalSales << endl
		 << "Number of customer                                  : " << numCustomer << endl
		 << "Most popular dish                                   : " << highestSaleFood << endl;
}

//**********************************************************************************************
// The writeFile function write the price, sales(quantity) of each food, total sales(RM),      *
// total number of customer	and the most popular dish into a file at the end of the program.   *
//**********************************************************************************************
void writeFile(string food[], float price[], int salesFood[], float totalSales, int numCustomer)
{

	int highest = salesFood[0];  // highest number of sales
	string highestSaleFood;		 // food with highest sales
	
	// For loop to find the most popular dish from the salesFood array and food array 
	for(int i = 0; i < SIZE; i++)
	{
		if(salesFood[i] > highest)
		{
			highest = salesFood[i];
			highestSaleFood = food[i]; // Assign highestSaleFood with the food name with highest sales	
		}
	}
	
	// File stream object
	ofstream outFile;
	
	// Open the file
	outFile.open("Sales.txt");
	
	outFile << "\n----------------------------------------------------------------------------\n"
	        << "No." << setw(21) << "Food" << "\tPrice(RM)" << "\tSales(Quantity)\n"
	        << "----------------------------------------------------------------------------\n";
	outFile << fixed << showpoint << setprecision(2);
	
	// For loop to write the array of food, price and salesFood in 3 columns into the file
	for(int i = 0; i < SIZE; i++)
	{
		outFile << setw(2) << i + 1 << ". " << setw(20) << food[i] << "\t" 
		        << price[i] << "\t\t" << salesFood[i] << endl;
	}
	outFile << "----------------------------------------------------------------------------\n";
	outFile << "Total sales (excluding service tax and delivery tax): RM" << totalSales << endl
			<< "Number of customer                                  : " << numCustomer << endl
			<< "Most popular dish                                   : " << highestSaleFood << endl;
			
	// Close the file
	outFile.close();	
}

