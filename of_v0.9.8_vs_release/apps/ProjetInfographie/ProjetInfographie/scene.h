#pragma once

#include <vector>
#include <memory> //shared_ptr
#include <iostream>
#include "primitive.h"


typedef std::shared_ptr<primitive> primitive_ptr;

class scene
{
public:
	scene();

	void addElement(size_t index, primitive_ptr& p, bool insertFirstChild);
	void removeElement(size_t index);
	
	friend std::ostream& operator<<(std::ostream& os, const scene& s);

private:

	class element {
	public:
		size_t getSize() const { return size; };
		size_t getIndex() const {	return index; };
		virtual void setIndex(size_t index) { this->index = index; };
		size_t getHeight() const { return height; };
		virtual void setHeight(size_t height) { this->height = height; };
		virtual std::string getType() const = 0;

		virtual size_t addElement(size_t index, primitive_ptr& p, bool insertFirstChild) = 0;
		virtual size_t removeElement(size_t index) = 0;

		friend std::ostream& operator<<(std::ostream& os, const element& e) { return e.print(os); };
	protected:
		size_t index;
		size_t size;
		size_t height;

		element(size_t index, size_t height);
		element(size_t index, size_t height, size_t size);

		virtual std::ostream& print(std::ostream& os) const;
	};
	
	typedef std::shared_ptr<element> element_ptr;

	class node : public element {
	public:
		node(size_t index, size_t height, primitive_ptr& p);

		std::string getType() const override;

		size_t addElement(size_t index, primitive_ptr& p, bool insertFirstChild) override;
		size_t removeElement(size_t index) override;
	protected:
		std::string contentType;
		primitive_ptr content;
	};

	typedef std::shared_ptr<node> node_ptr;

	class group : public element {
	public:
		group(size_t index, size_t height);

		void setIndex(size_t index) override;
		void setHeight(size_t height) override;
		std::string getType() const override;

		size_t addElement(size_t index, primitive_ptr& p, bool insertFirstChild) override;
		size_t removeElement(size_t index) override;

	protected:
		std::vector<element_ptr> childrens;

		std::ostream& print(std::ostream& os) const override;
	};

	typedef std::shared_ptr<group> group_ptr;

	group root;
};

