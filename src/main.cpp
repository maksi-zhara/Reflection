#include<iostream>
#include<TypeInfo.h>

REGISTER_TYPE(int)
REGISTER_TYPE(float)
class A{};
int main() {
	for(auto& pair : TypeId::GetAllTypeInformation()) 
	{
		std::cout << "Id: " << pair.first << " Name: " << pair.second.name << '\n';
	}
	return 0;
}
