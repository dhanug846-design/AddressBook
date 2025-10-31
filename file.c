#include <stdio.h>
#include "file.h"

void saveContactsToFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("contacts.csv", "w");  // Opening file in writing mode
    if (!fptr)  // check if file opening failed  
    {
        printf("Could not open contacts.csv for writing.\n");
        return;
    }

    // Write each contact as a CSV line: name, phone, email
    for (int i = 0; i < addressBook->contactCount; i++)
    {
        fprintf(fptr, "%s,%s,%s\n",
            addressBook->contacts[i].name,
            addressBook->contacts[i].phone,
            addressBook->contacts[i].email);
    }

    fclose(fptr);  // Close the file
    printf("Saved %d contacts to contacts.csv\n", addressBook->contactCount);
}

void loadContactsFromFile(AddressBook *addressBook) 
{
    FILE *fptr = fopen("contacts.csv", "r");  // Opening file in read mode
    if (!fptr)
    { 
        return; // File not found
    }
    addressBook->contactCount = 0;

    while ( fscanf(fptr, "%[^,], %[^,], %[^\n]\n",
            addressBook->contacts[addressBook->contactCount].name,
            addressBook->contacts[addressBook->contactCount].phone,
            addressBook->contacts[addressBook->contactCount].email
            ) == 3 )  // fscanf returns number of successfully read iteams(should be 3)
            {
                addressBook->contactCount++;  // Increment contact count
            }
            fclose(fptr);  // closing the file after reading
}
