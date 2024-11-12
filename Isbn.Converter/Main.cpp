#include <iostream>
#include <string>

std::string isbnTenToThirteen(std::string& isbn);
std::string isbnThirteenToTen(std::string& isbn);

int main()
{
	std::cout << "Enter ISBN: " << std::endl;

	// string is used to account for the possibility of an X in a 10-char ISBN
	std::string isbnToConvert;
	std::cin >> isbnToConvert;
	
	if (isbnToConvert.size() == 13)
	{
		std::string isbnTo10 = isbnThirteenToTen(isbnToConvert);
		if (!isbnTo10.empty())
		{
			std::cout << "Converted to ISBN-10: " << isbnTo10 << std::endl;
		}
	}
	else if (isbnToConvert.size() == 10)
	{
		std::string isbnTo13 = isbnTenToThirteen(isbnToConvert);
		if (!isbnTo13.empty())
		{
			std::cout << "Converted to ISBN-13: " << isbnTo13 << std::endl;
		}
	}
	else
	{
		std::cout << "ISBN incorrect length." << std::endl;
	}
}

std::string isbnTenToThirteen(std::string& isbn)
{
	if (isbn.size() != 10)
	{
		std::cerr << "ISBN is not 10 digits" << std::endl;
		return "";
	}

	//validate the first 9 digits 
	for (int i{ 0 }; i < 9; i++)
	{
		if (!std::isdigit(isbn.at(i)))
		{
			std::cerr << "ISBN is invalid. Contains Non-Digit characters." << std::endl;
			return "";
		}
	}

	//validating last digit is a digit or X
	char lastChar{ isbn.back() };
	if (!std::isdigit(lastChar) && lastChar != 'X' && lastChar != 'x')
	{
		std::cerr << "ISBN is invalid. Last character is not a digit or X." << std::endl;
		return "";
	}

	//Step 1: drop the last character(9 digits)
	isbn.pop_back();

	//Step 2: tack on "978" to the front(12 digits)
	isbn.insert(0, "978");

	//Step 3: calculate the checksum
	int checksum{ 0 };
	for (size_t i{ 0 }; i < isbn.size(); i++)
	{
		//convert char to int
		int digit{ isbn.at(i) - '0' };

		//check if the digit is even or odd
		if (i % 2 == 0)
		{
			// We multiply the even digits by 1,
			digit = digit * 1;
		}
		else
		{
			// multiply the odd digits by 3, 
			digit = digit * 3;
		}
		/*
		* could also be written as:
		* checksum += (i % 2 == 0) ? digit : digit * 3;
		* I've been given conflicting information on which is more readable from different sources
		*/

		//adding digits to get the checksum for mod 10
		checksum += digit;
	}

	//mod 10 to get the checksum
	checksum = checksum % 10;

	if (checksum == 0)
	{
		checksum = 0;
	}
	else
	{
		checksum = 10 - checksum;
	}

	//Step 4: tack on the checksum to the end of the isbn
	isbn += std::to_string(checksum);
	return isbn;
}

std::string isbnThirteenToTen(std::string& isbn)
{
	if (isbn.size() != 13)
	{
		std::cerr << "ISBN is not 13 digits" << std::endl;
		return "";
	}

	// Check that the ISBN-13 starts with "978"
	if (isbn.substr(0, 3) != "978")
	{
		std::cerr << "ISBN-13 does not start with '978' and cannot be converted to ISBN-10." << std::endl;
		return "";
	}

	// Validate that all characters are digits
	for (size_t i = 0; i < isbn.size(); ++i)
	{
		if (!std::isdigit(isbn.at(i)))
		{
			std::cerr << "ISBN contains invalid characters." << std::endl;
			return "";
		}
	}

	// Step 1: Remove the 978 from the front
	isbn.erase(0, 3);

	//Step 2 : Remove the last digit
	isbn.pop_back();

	int checksum{ 0 };
	for (int i{ 0 }; i < 9; i++)
	{
		//convert char to int
		int digit{ isbn.at(i) - '0' };
		//multiply the digit by the position
		checksum += digit * (i + 1);
	}

	//if the checksum is 10, we need to add an X
	int remainder{ checksum % 11 };
	char checkDigit;

	if (remainder == 10)
	{
		checkDigit = 'X';
	}
	else
	{
		checkDigit = remainder + '0';
	}

	isbn += checkDigit;
	return isbn;
}
