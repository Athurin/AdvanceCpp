#pragma once
#ifndef __COMMENT_H__
#define __COMMENT_H__
#include <QString>

class Comment  //评论类 接收解析好的Json数据 打包成类
{
public:
	int getPostId() const;
	int getId() const;
	QString getName() const;
	QString getEmail() const;
	QString getBody() const;

	void setPostId(int pid);
	void setId(int id);
	void setName(QString name);
	void setEmail(QString email);
	void setBody(QString body);


	//暂时不需要set方法

private:
	int m_postId;
	int m_Id;
	QString m_name;
	QString m_email;
	QString m_body;

};

#endif // !__COMMENT_H__