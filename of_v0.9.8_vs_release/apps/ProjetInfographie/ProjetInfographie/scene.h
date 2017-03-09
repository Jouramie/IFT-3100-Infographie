#pragma once

#include <vector>
#include <memory> //shared_ptr
#include "primitive.h"


typedef std::shared_ptr<primitive> primitive_ptr;

class scene
{
public:
	scene();

	void addElement(primitive_ptr& p);
	void addElement(size_t index, primitive_ptr& p, bool insertFirstChild);
	void removeElement(size_t index);
	void selectElement(size_t index);
	
	//TODO operator<<

private:

	class element {
	public:
		size_t getSize();
		size_t getIndex();
		void setIndex(size_t index);

		virtual void addElement(size_t index, primitive_ptr& p, bool insertFirstChild);
		virtual size_t removeElement(size_t index);
	protected:
		size_t index;
		size_t size;

		element(size_t index);
		element(size_t index, size_t size);
	};
	
	typedef std::shared_ptr<element> element_ptr;

	class node : public element {
	public:
		node(size_t index, primitive_ptr& p);

		void addElement(size_t index, primitive_ptr& p, bool insertFirstChild);
		size_t removeElement(size_t index);
	protected:
		primitive_ptr content;
	};

	class group : public element {
	public:
		group(size_t index);

		void setIndex(size_t index);

		void addElement(size_t index, primitive_ptr& p, bool insertFirstChild);
		size_t removeElement(size_t index);
	protected:
		std::vector<element_ptr> childrens;
	};

	group root;
	size_t selectedIndex;
};

