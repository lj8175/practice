// =====================================================================================
//
//       Filename:  Singleton.h
//
//    Description:  单件的基类
//
//        Created:  2012年08月01日
//         Author:  xandylai (xandylai@tencent.com)
//        Company:  tencent
//
//        History:
//        <author>	<time>		<version>	<desc>
//
//        Copyright (c) 2012, tencent
//
// =====================================================================================
#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <typename T>
class Singleton
{
protected:
    static T* m_instance;	// Actual ptr the the instance
public:
    // Constructor, set the 'm_instance'
    // Can not be called more than once
    Singleton()
    {
         m_instance = static_cast<T*>(this);
    }
    ~Singleton()
    {
         m_instance = NULL;
    }

    static T& GetInstance()
    {
        return (*m_instance);
    }

    static T* GetInstancePtr()
    {
        return m_instance;
    }
};

template <typename T> 
T* Singleton<T>::m_instance = NULL;
#endif
