#include<TypeHash.h>
#include<map>
#include<cassert>
struct TypeInfo final
{
	std::string_view name{};
	size_t size{};
	size_t align{};

	template<typename T> static constexpr TypeInfo Create();
};

class TypeId final
{
public:
	constexpr TypeId(uint64_t Id) : ID(Id) {}
	constexpr TypeId()=default;
public:
	constexpr void setId(uint64_t Id) { ID = Id; } 
	constexpr uint64_t getId() const { return ID; }
	const TypeInfo& getTypeInfo() const { return getTypeInformation(*this); }
	const std::string_view getName() const { return getTypeInfo().name; }
	const size_t getSize() const { return getTypeInfo().size; }
	const size_t getAlign() const { return getTypeInfo().align; }

	template<typename T> static constexpr TypeId Create();
	template<typename T> static TypeInfo& RegisterTypeId() 
	{
		auto& typeInfos = GetStatics().typeInfos;
		const uint64_t typeId = TypeId::Create<T>().getId();
		assert(!typeInfos.contains(typeId));
		return typeInfos.emplace(typeId, TypeInfo::Create<T>()).first->second;//NOTE:first->second??
	}
	static const TypeInfo& getTypeInformation(TypeId typeId) //NOTE:maybe pointer???
	{
		assert(GetStatics().typeInfos.contains(typeId.getId()));
		return GetStatics().typeInfos[typeId.getId()];
	}
	static const auto& GetAllTypeInformation() 
	{
		return GetStatics().typeInfos;
	}
private:
	uint64_t ID{};
	struct StaticData
	{
		std::unordered_map<uint64_t, TypeInfo> typeInfos{};
	};
	static StaticData& GetStatics() 
	{
		static StaticData data{};
		return data;
	}
};

template<typename T> inline constexpr TypeInfo TypeInfo::Create() 
{
	TypeInfo info{};
	info.name = Reflection::TypeName<T>();
	info.size = sizeof(T);
	info.align = alignof(T);
	return info;
}

template<typename T> inline constexpr TypeId TypeId::Create() 
{
	return TypeId(Reflection::TypeId<T>());
}

template<typename T>
class RegisterType
{
private:
	class RegisterTypeOnce
	{
	public: 
		RegisterTypeOnce()
		{
			TypeId::RegisterTypeId<T>();
		}
	};
public:
	inline static RegisterTypeOnce Registerer{};
};

#define _REGISTER_TYPE_INTERNAL(TYPE, VARNAME) RegisterType<TYPE> VARNAME##TYPE {};
#define REGISTER_TYPE(TYPE) _REGISTER_TYPE_INTERNAL(TYPE, RegisterType_)
