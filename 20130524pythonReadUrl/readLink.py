#!/usr/bin/env python
#coding=utf-8
'''
Created on 2013-5-24

@author: joelliu

$("#u",$("#frame")[0].contentDocument).val("");
$("#p",$("#frame")[0].contentDocument).val("");
$("#login_button",$("#frame")[0].contentDocument).click();

'''

import urllib2,cookielib,httplib,Cookie,urllib,re
from hashlib import md5

def encryptPassword(p, v, u):  
    '''对密码进行加密'''  
    return 
   
def check_appid(appid):
    
    if appid == "":
        '''登录'''
        app='www..com'
        url="http%3A%2F%2Fwww..com%2F" 
        connection1="...com"
        connection2="...com"
        connection3="..com"
        refer3="" 
    app_tuple=(appid,app,url,connection1,connection2,connection3,refer3)
    return app_tuple
  



def browserLogin(appid, uin, p, qurl):
    #登陆页面，可以通过抓包工具分析获得，如fiddler，wireshark
    app_tuple=check_appid(appid)
    appid,app,url,connection1,connection2,connection3,refer3=app_tuple
    try:
        #获得一个cookieJar实例
        cj = cookielib.CookieJar()
        #cookieJar作为参数，获得一个opener的实例
        opener=urllib2.build_opener(urllib2.HTTPCookieProcessor(cj))
        
        refer2=""
        queryset2=""
        #伪装成一个正常的浏览器，避免有些web服务器拒绝访问。
        opener.addheaders = [('User-agent','Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1)')]
        opener.addheaders = [('Referer',refer2)]
        op = opener.open('http://'+connection2+queryset2)
        ptui_checkVC = op.read()
        print ptui_checkVC
        opener.addheaders = [('Referer',refer3)]
        listVC = map(lambda x: x.strip('\''), ptui_checkVC[13:-2].split(','))
        verifyCode = listVC[1]    
        ptUin = listVC[2]  
        ptuinStr = ''.join([chr(long(i, 16)) for i in ptUin.split(r'\x') if i != ''])  
        enp = encryptPassword(p, verifyCode, ptuinStr)
        queryset3 = ""
        op = opener.open('http://'+connection3+queryset3)
        print op.read()
        #以带cookie的方式访问页面
        op = opener.open(qurl)
        #读取页面源码
        data= op.read()
        return data
    except Exception,e:
        print str(e)



if __name__ == '__main__':
    
    uin, p = "", ""

    l = ["http://www..com/index.php",
         ]
    a = browserLogin("",uin,p, l[0])
    p = re.compile(r"<a.+?href=.+?>.+?</a>")
    phref = re.compile(r"(?<=href\=\").*?(?=\")")
    psrc = re.compile(r"(?<=src\=\").*?(?=\")")
    for x in  phref.findall(a) :
        print x
    for x in psrc.findall(a):
        print x
        
