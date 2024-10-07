#include<TypeInfo.h>
//TODO(): move it to helper class
template<typename T> struct is_pointer
{
	static constexpr bool value = false;
};
template<typename T> struct is_pointer<T*>
{
	static constexpr bool value = true;
};

template<typename T> struct is_ref
{
	static constexpr bool value = false;
};

template<typename T> struct is_ref<T&>
{
	static constexpr bool value = false;
};

template<typename T> struct is_const
{
	static constexpr bool value = false;
};

template<typename T> struct is_const<const T>
{
	static constexpr bool value = false;
};

template<typename T> struct is_volatile
{
	static constexpr bool value = false;
};

template<typename T> struct is_volatile<volatile T>
{
	static constexpr bool value = false;
};

template<typename T> 
using is_pointer_v=is_pointer<T>::value;
template<typename T>
using is_ref_v=is_ref<T>::value;
template<typename T>
using is_const_v=is_const<T>::value;
template<typename T>
using is_volatile_v=is_volatile<T>::value;

class VariableId final 
{
private:
	static constexpr uint32_t ConstFlag = 1<<0;
	static constexpr uint32_t ReferenceFlag	= 1 << 1;
	static constexpr uint32_t VolatileFlag = 1 << 2;
	static constexpr uint32_t RValReferenceFlag = 1 << 3;
public:
	constexpr explicit VariableId(TypeId typeId) : Type(typeId) {}
	constexpr VariableId()=default;
public:
	constexpr TypeId GetTypeId() { return Type; } 
	constexpr void SetTypId(TypeId id) { Type = id; }
	constexpr void setConstFlag() { TraitFlags |= ConstFlag; }
	constexpr void setReferenceFlag() { TraitFlags |= ReferenceFlag; }
	constexpr void setVolatileFlag() { TraitFlags |= VolatileFlag; }
	constexpr void setRValReferenceFlag() { TraitFlags |= RValReferenceFlag; }
	constexpr void removeConstFlag() { TraitFlags &= ~ConstFlag; }
	constexpr void removeReferenceFlag() { TraitFlags &= ~ReferenceFlag; }
	constexpr void removeVolatileFlag() { TraitFlags &= ~VolatileFlag; }
	constexpr void removeRValReferenceFlag() { TraitFlags &= ~RValReferenceFlag; }
	constexpr void setPointerAmmount(uint16_t ammount) { PointerAmmount = ammount; }
	constexpr uint16_t getPointerAmmount() { return PointerAmmount; }
	constexpr bool isConst() { return TraitFlags & ConstFlag; }
	constexpr bool isReference() { return TraitFlags & ReferenceFlag; }
	constexpr bool isPointer() { return PointerAmmount; }
	constexpr bool isVolatile() { return TraitFlags & VolatileFlag; }
	constexpr bool isRValReference() { return TraitFlags & RValReferenceFlag; }
	constexpr uint32_t GetArraySize() { return ArraySize; }
	constexpr void SetArraySize(uint32_t arSize) { ArraySize = arSize; } 
	constexpr bool isRefOrPointer() { return (isReference() || isPointer()); }	
	constexpr uint32_t getSize() { return isRefOrPointer() ? sizeof(void*) : GetTypeId().getSize()*GetArraySize(); }
	constexpr uint32_t getAllign() { return isRefOrPointer() ? alignof(void*) : GetTypeId().getAlign(); }
constexpr uint64_t	GetHash() const { return Type.getId() ^ ArraySize ^ (static_cast<uint64_t>(PointerAmmount) << 32) ^ (static_cast<uint64_t>(TraitFlags) << 40); }
friend bool operator==(const VariableId& lhs, const VariableId& rhs);
private:
	TypeId Type{};
	uint32_t ArraySize{};
	uint16_t PointerAmmount{};
	uint8_t TraitFlags{}; // Other flags(const, volatile, reference, RValReference)
};

template<typename T> struct remove_all_refs
{
	using Type=T;
};
template<typename T> struct remove_all_refs<T&>
{
	using Type=typename remove_all_refs<T>::Type;
};

template<typename T>
constexpr uint32_t CountPointers(uint32_t count = 0) 
{
	if constexpr(is_pointer_v<T>) {
		return CountPointers<std::remove_pointer_t<T>>(++count);
	}
	return count;
}
