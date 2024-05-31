#include <iostream>
#include <string>

int main()
{
    char choice;
    
    std::cout << "Welcome to Resaucer!\n1- Convert File\n2- Convert Folder\n3- Exit\n";
    std::cin >> choice;

    switch (choice)
    {
        case '1':
        {
            std::string file;
            std::cout << "Enter the file path: ";
            std::cin >> file;
            break;
        }
        case '2':
        {
            std::string folder;
            std::cout << "Enter the folder path: ";
            std::cin >> folder;
            break;
        }
        case '3':
        {
            return 0;
        }
    }
    
    return 0;
}
