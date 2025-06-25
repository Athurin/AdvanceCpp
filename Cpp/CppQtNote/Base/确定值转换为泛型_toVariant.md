
下面的代码可以将常见的基本类型转换为QVariant（泛型），如果是自定义的类型要转换为泛型，需要先注册。
```cpp
//定义模板，转换泛型Variant
template<typename T>
inline QVariant _toVariant(const T& data = T()) //可以接受形参为空的情况，提供默认值 T()
{
	return QVariant::fromValue(data);
}
```

虽然说当参数为空的时候，会提供一个默认的参数，但是，如果不提供模板`T`的类型任然会报错。
```CPP
//这段代码会报错
settings.setValue(fromChString("ServerIP"), _toVariant());


//这段不会，可以传参为空，此时使用int()类型的默认构造，data的值应为0
settings.setValue(fromChString("ServerIP"), _toVariant<int>());
```

