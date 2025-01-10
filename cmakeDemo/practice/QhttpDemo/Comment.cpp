#include "Comment.h"


int Comment::getPostId() const
{
	return m_postId;
}

int Comment::getId() const
{
	return m_Id;
}

QString Comment::getName() const
{
	return m_name;
}

QString Comment::getEmail() const
{
	return m_email;
}

QString Comment::getBody() const
{
	return m_body;
}

void Comment::setPostId(int pid)
{
	m_postId = pid;
}

void Comment::setId(int id)
{
	m_Id = id;
}

void Comment::setName(QString name)
{
	m_name = name;
}
void Comment::setEmail(QString email)
{
	m_email = email;
}

void Comment::setBody(QString body)
{
	m_body = body;
}