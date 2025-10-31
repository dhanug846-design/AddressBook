#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contact.h"
#include "file.h"
#include "populate.h"

void listContacts(AddressBook *addressBook, int sortCriteria) 
{
    // Sort contacts based on the chosen criteria
    if(addressBook->contactCount== 0)
    {
        printf("No contacts available.\n");
        return;
    }

    for(int i = 0; i < addressBook->contactCount - 1; i++)
    {
        for(int j = 0; j < addressBook->contactCount-i-1; j++)
        {
            int compare = 0;
            if(sortCriteria == 1)
            {
                compare = strcmp(addressBook->contacts[i].name, addressBook->contacts[j + 1].name);  // Sort by name
            }
            else if(sortCriteria == 2)
            {
                compare = strcmp(addressBook->contacts[i].phone, addressBook->contacts[j + 1].phone);  // Sort by phone no
            }
            else if(sortCriteria == 3)
            {
                compare = strcmp(addressBook->contacts[i].email, addressBook->contacts[j + 1].email);  // sort by email
            }
            if(compare > 0)
            {   // Swaping
                Contact temp = addressBook->contacts[i];
                addressBook->contacts[i] = addressBook->contacts[j];
                addressBook->contacts[j] = temp;
            }
        }
    }

    printf("-----------------------------------Contact List-----------------------------------\n");
    printf("%6s %-30s %-20s %-30s\n", "Index", "Name", "Phone No", "Email");  // heards
    printf("==================================================================================\n");

    for(int i = 0; i < addressBook->contactCount; i++)
    {  // printing contacts
        printf("%6d. %-30s %-20s %-30s\n", i + 1 , 
            addressBook->contacts[i].name, 
            addressBook->contacts[i].phone, 
            addressBook->contacts[i].email);
    }

    printf("----------------------------------------------------------------------------------\n");
}

void initialize(AddressBook *addressBook) {
    addressBook->contactCount = 0;
    //populateAddressBook(addressBook);
    
    // Load contacts from file during initialization (After files)
    loadContactsFromFile(addressBook);  // load from file
}

void saveAndExit(AddressBook *addressBook) {
    saveContactsToFile(addressBook); // Save contacts to file
    exit(EXIT_SUCCESS); // Exit the program
}


void createContact(AddressBook *addressBook)
{
    if(addressBook->contactCount >= MAX_CONTACTS)
    {
        printf("AddressBook is full, Cannot add more cantacts\n");
        return;
    }
    
    printf("\n");

    Contact newcontact;  // temprary new contact
    printf("Enter Name: ");
    scanf(" %[^\n]", newcontact.name); //Reads the entered new name

    // Checks the newcontact or name if it is valid or not
    if(!validate_name(newcontact.name, addressBook))
    {
        printf("Invalid name or duplicate email, only alphabets and spaces allowed\n"); // if it's invalid it prints Invalid name
        return;
    }

    printf("Enter Phone Number: ");
    scanf(" %[^\n]", newcontact.phone);  // Reads the entered new phone number

    // Checks if the entered phone number is valid or not
    if(!validate_phone(newcontact.phone, addressBook))
    {
        printf("Invalid phone number, must be 10 digits and unique\n"); // prints if it's invalid number
        return;
    }

    printf("Enter email: ");
    scanf(" %[^\n]", newcontact.email);  // Reads the entered new email
    // checks if the entered email is valid r not
    if(!validate_email(newcontact.email, addressBook))
    {
        printf("Invalid email format or duplicate email\n");
        return;
    }

    addressBook->contacts[addressBook->contactCount++] = newcontact;  // saving contact
    printf("contact added successfully\n");

    listContacts(addressBook, 4);  // TO display the contacts after successfully created
}

int searchContact(AddressBook *addressBook) 
{   // to search
    char search[100];
    printf("Enter Name/Phone Number/email: ");
    scanf(" %[^\n]", search);

    for(int i = 0; i < addressBook->contactCount; i++)
    {
        if(strcasestr(addressBook->contacts[i].name, search) != NULL ||
            strcasestr(addressBook->contacts[i].phone, search) != NULL||
            strcasestr(addressBook->contacts[i].email, search) != NULL)  // comparing strcasestr helps to find both lower case & upper case alphanumaric
            {
                //prints the searched contacts if it's found
                printf("%3d %-20s  %-20s  %-20s\n", i + 1, 
                    addressBook->contacts[i].name, 
                    addressBook->contacts[i].phone, 
                    addressBook->contacts[i].email);
                return i;  //return index of the match
            }
    }
    
    printf("\n");   // new line
    printf("NO contact found with '%s'\n", search);
    return -1;
}

void editContact(AddressBook *addressBook)
{
    char new_name[50];  // Buffer for update name
    char new_number[20]; // Buffer for update phone no
    char new_email[50];  // Buffer for update email

    printf("\n");

    int index = searchContact(addressBook);

    if (index < 0) //chech's the index
    {
        printf("Contact not found.\n");
        return;
    }

    int choice, value;
    printf("Enter the choice: \n");
    printf("1. Edit Name\n2. Edit Phone Number\n3. Edit Email\n");
    scanf("%d", &choice);

    // to edit name
    if(choice == 1)
    {
        printf("Enter Updated Name: ");
        scanf(" %[^\n]", new_name);

        if (strlen(new_name) > 0) //validate and update
        {
            if(!validate_name(new_name, addressBook))
            {
                printf("Invalid name, only alphabets and spaces allowed\n");
                return;
            } 
            strcpy(addressBook->contacts[index].name, new_name);
        }
    }
    // to edit phone number
    if(choice == 2)
    {
        printf("Enter Updated phone_number\n");
        scanf(" %[^\n]", new_number);

        if (strlen(new_number) > 0) //validate and update
        {
            if(!validate_phone(new_number, addressBook))
            {
                printf("Invalid phone number, must be 10 digits and unique\n");
                return;
            }
            strcpy(addressBook->contacts[index].phone, new_number);
        }
    }
    // to edit email
    if(choice == 3)
    {
        printf("Enter Updated Email\n");
        scanf(" %[^\n]", new_email);

        if(strlen(new_email) > 0)  //validate and update
        {
            if(!validate_email(new_email, addressBook))
            {
                printf("Invalid email formate or duplicate email\n");
                return;
            }
            strcpy(addressBook->contacts[index].email, new_email);
        }
    }

    printf("Contact updated successfully.\n");
    listContacts(addressBook, 4);
}

void deleteContact(AddressBook *addressBook)
{
    if(addressBook->contactCount == 0)  //chech's the count of the contacts
    {
        printf("No contacts available to delet\n");
        return;
    }

    searchContact(addressBook);  // TO search the contacts and display

    printf("\n");  // for newline

    int index;
    printf("Enter the index of contact to delete: ");
    scanf("%d", &index);

    // checks the index
    if(index < 0 || index > addressBook->contactCount)
    {
        printf("Invalid index\n");
        return;
    }

    int actualindex = index - 1; //makes the index to starts from 0

    printf("Deleting: %-20s %-20s %-20s\n", 
        addressBook->contacts[actualindex].name, 
        addressBook->contacts[actualindex].phone,
        addressBook->contacts[actualindex].email);

    for(int i = actualindex; i < addressBook->contactCount - 1; i++)
    {
        addressBook->contacts[i] = addressBook->contacts[i + 1]; 
    }

    addressBook->contactCount--;  // to reduce count

    printf("\n");  // new line
    printf("contact deleted successfully\n");
    listContacts(addressBook, 4);  // updated list
}

int validate_name(char *name, AddressBook *addressbook)
{
    for(int i = 0; name[i] != '\0';i++)
    {
        if(!((name[i] >= 'A' && name[i] <= 'Z') ||
             (name[i] >= 'a' && name[i] <= 'z') ||
              name[i] == ' '))  // to check alphabets or spaces
        {
            return 0;
        }
    }
    
    //checking for duplicate name
    for(int i = 0; i < addressbook->contactCount; i++)
    {
        if(strcmp(name, addressbook->contacts[i].name) == 0)  // comparing(using strcmp)
        {
            return 0;  // duplicate found
        }
    }
    return 1; // valid
}

int validate_phone(char *phone, AddressBook *addressbook)
{
    int len = strlen(phone);
    
    if(len != 10) // checking if phone no has 10 digits r not
    {
        return 0;
    }

    for(int i = 0; i < len; i++)
    {
        if(phone[i] < '0' || phone[i] > '9')  // only digits
        {
            return 0;
        }
    }
    //checking for duplicate phone no
    for(int i = 0; i < addressbook->contactCount; i++)
    {
        if(strcmp(phone, addressbook->contacts[i].phone) == 0)  // comparing phone number
        {
            return 0;  // found duplicate 
        }
    }
    return 1; // valid
}

int validate_email(char *email, AddressBook *addressbook)
{
    //To check email is duplicate or not
    for(int i = 0; i < addressbook->contactCount; i++)
    {
        if(strcmp(email, addressbook->contacts[i].email) == 0)  // comparing email's
        {
            return 0;  // found duplicate
        }
    }

    const char *at = strchr(email, '@'); //to check the '@' is present or not 
    const char *dot = strchr(email, '.');  // must have '.'

    if(at > dot || at == NULL || dot == NULL)
    {
        return 0;
    }
    
    for(int i = 0; email[i]; i++)
    {
        if (!( (email[i] >= 'a' && email[i] <= 'z')||
        (email[i] >= '0' && email[i] <= '9')||
        (email[i] == '@')||
        (email[i] == '.')))   
        {
            return 0; // Invalid character found
        }
    }
    return 1; // Valid email
}

