#include "scene.h"
#include <string>

using namespace std;

scene::scene() : root{ new group{ 0, 0} } {}

void scene::addElement(size_t index, const primitive2d& p, bool insertFirstChild)
{
	if (index == 0 && !insertFirstChild)
	{
		throw invalid_argument("root don't have parent...");
	}
	primitive2d* prim = new primitive2d{ p };

	addElement(index, primitive_ptr{ prim }, insertFirstChild);
}

void scene::addElement(size_t index, const primitive3d& p, bool insertFirstChild)
{
	if (index == 0 && !insertFirstChild)
	{
		throw invalid_argument("root don't have parent...");
	}
	addElement(index, primitive_ptr{ new primitive3d{ p } }, insertFirstChild);
}

void scene::addElement(size_t index, const primitiveTopo& p, bool insertFirstChild)
{
	if (index == 0 && !insertFirstChild)
	{
		throw invalid_argument("root don't have parent...");
	}
	addElement(index, primitive_ptr{ new primitiveTopo{ p } }, insertFirstChild);
}

void scene::addElement(size_t index, const forme3d& p, bool insertFirstChild)
{
	if (index == 0 && !insertFirstChild)
	{
		throw invalid_argument("root don't have parent...e");
	}
	addElement(index, primitive_ptr{ new forme3d{ p } }, insertFirstChild);
}

void scene::addElement(size_t index, const extModel& p, bool insertFirstChild)
{
	if (index == 0 && !insertFirstChild)
	{
		throw invalid_argument("root don't have parent...e");
	}
	addElement(index, primitive_ptr{ new extModel{ p } }, insertFirstChild);
}

void scene::addElement(size_t index, primitive_ptr& p, bool insertFirstChild)
{
	if (index == 0 && !insertFirstChild)
	{
		throw invalid_argument("root don't have parent...");
	}
	root->addElement(index, p, insertFirstChild);
}

void scene::removeElement(size_t index) 
{
	if (index == 0) {
		throw invalid_argument("can't remove root...");
	}
	root->removeElement(index);
}

void scene::clearElements() 
{
	root = group_ptr{ new group{0, 0} };
}

scene::scene_iterator scene::begin()
{
	return scene_iterator{ root, 0 };
}

scene::scene_iterator scene::end()
{
	return scene_iterator{ root, root->getSize() + 1};
}


ostream & operator<<(ostream & os, const scene & s)
{
	return os << *s.root;
}

/************************************************************************/
/*							SceneGroup		                            */
/************************************************************************/


scene::group::group(size_t index, size_t height) : element(index, height, 0) {}

void scene::group::setIndex(size_t index)
{
	for (auto& i : childrens) {
		i->setIndex(i->getIndex() + (index - this->index));
	}
	element::setIndex(index);
	
}

void scene::group::setHeight(size_t height)
{
	for (auto& i : childrens) {
		i->setHeight(i->getHeight() + (height - this->height));
	}
	element::setHeight(height);
}

std::string scene::group::getType() const
{
	if (index == 0) {
		return "root";
	}
	else {
		return "group";
	}
}

/*
Ajouter l'élément directement après l'élément à l'index en paramètre
*/
size_t scene::group::addElement(size_t index, primitive_ptr& p, bool insertFirstChild)
{
	size_t addedSize = 0;

	if (this->index == index) {
		if (insertFirstChild)
		{
			//Inserer comme premier element
			childrens.insert(childrens.begin(), element_ptr{ new node{ index + 1, height + 1, p } });
			for (auto& it = childrens.begin() + 1; it < childrens.end(); ++it) {
				it->get()->setIndex(it->get()->getIndex() + 1);
			}
			addedSize++;
		}
		else {
			throw invalid_argument("element must to be add in the parent");
		}
	}
	else {
		size_t ubound = childrens.size();
		size_t lbound = 0;
		size_t i;

		while (lbound <= ubound) {
			i = lbound + (ubound - lbound) / 2;
			if (childrens[i]->getIndex() == index) {
				if (insertFirstChild) {
					if (childrens[i]->getType() != "group") {
						group_ptr temp = group_ptr{ new group{ index, height + 1 } };
						temp->childrens.push_back(childrens[i]);
						temp->childrens[0]->setIndex(index + 1);
						temp->childrens[0]->setHeight(height + 2);
						temp->size = temp->childrens[0]->getSize() + 1;
						childrens[i] = temp;
						addedSize++;
					}
					addedSize += childrens[i]->addElement(index, p, insertFirstChild);
				}
				else {
					childrens.insert(childrens.begin() + i + 1, element_ptr{ new node{ index + childrens[i]->getSize(), height + 1, p } });
					i++;
					addedSize++;
				}
				i++;
				break;
			}
			else if (childrens[i]->getIndex() < index) {
				lbound = i + 1;
				if (ubound < lbound) {
					//Ajoute l'élément dans le groupe sous-jacent
					addedSize += childrens[i]->addElement(index, p, insertFirstChild);
					i++;
					break;
				}
			}
			else {
				ubound = i - 1;
				if (ubound < lbound) {
					//Ajoute l'élément dans le groupe sous-jacent
					addedSize += childrens[i - 1]->addElement(index, p, insertFirstChild);
					break;
				}
			}
		}	
		for (auto& it = childrens.begin() + i; it < childrens.end(); ++it) {
			it->get()->setIndex(it->get()->getIndex() + addedSize);
		}
	}
	size += addedSize;
	return addedSize;
}

// return the size of the removed elements
size_t scene::group::removeElement(size_t index)
{
	size_t removedSize = 0;
	
	if (this->index == index) {
		//Retirer tous les enfants
		for (auto& i : childrens) {
			i->removeElement(i->getIndex());
		}
		removedSize += size;
		childrens.clear();
	}
	else {
		size_t ubound = childrens.size();
		size_t lbound = 0;
		size_t i;

		while (lbound <= ubound) {
			i = lbound + (ubound - lbound) / 2;
			if (childrens[i]->getIndex() == index) {
				//Retirer l'élément i
				removedSize += childrens[i]->removeElement(index);
				childrens.erase(childrens.begin() + i);
				break;
			}
			else if (childrens[i]->getIndex() < index) {
				lbound = i + 1;
				if (ubound < lbound) {
					//Retirer l'élément dans le groupe sous-jacent
					removedSize += childrens[i]->removeElement(index);
					i++;
					break;
				}
			}
			else {
				ubound = i - 1;
				if (ubound < lbound) {
					//Retirer l'élément dans le groupe sous-jacent
					removedSize += childrens[i - 1]->removeElement(index);
					break;
				}
			}
		}
		for (auto& it = childrens.begin() + i; it < childrens.end(); ++it) {
			it->get()->setIndex(it->get()->getIndex() - removedSize);
		}
	}
	size -= removedSize;
	return removedSize;
}

scene::element* scene::group::getElement(size_t index)
{
	if (this->index == index) {
		return this;
	}
	else {
		element* elem;
		size_t ubound = childrens.size();
		size_t lbound = 0;
		size_t i;

		while (lbound <= ubound) {
			i = lbound + (ubound - lbound) / 2;
			if (childrens[i]->getIndex() == index) {
				elem = childrens[i]->getElement(index);
				break;
			}
			else if (childrens[i]->getIndex() < index) {
				lbound = i + 1;
				if (ubound < lbound) {
					//Retirer l'élément dans le groupe sous-jacent
					elem = childrens[i]->getElement(index);
					i++;
					break;
				}
			}
			else {
				ubound = i - 1;
				if (ubound < lbound) {
					//Retirer l'élément dans le groupe sous-jacent
					elem = childrens[i - 1]->getElement(index);
					break;
				}
			}
		}
		return elem;
	}
}

std::ostream & scene::group::print(std::ostream & os) const
{
	element::print(os);
	for (auto& i : childrens) {
		os << endl << *i;
	}
	return os;
}


/************************************************************************/
/*							SceneNode		                            */
/************************************************************************/

scene::node::node(size_t index,size_t height, primitive_ptr& p) : element(index, height, 1), content{ p }, contentType{ "primitive" } { }

std::string scene::node::getType() const
{
	return contentType;
}

size_t scene::node::addElement(size_t index, primitive_ptr& p, bool insertFirstChild)
{
	if (index != this->index) {
		throw invalid_argument("index need to be equals to the index of the node");
	}
	if (insertFirstChild) {
		throw invalid_argument("node need to be wraped in a group");
	}
	this->content = p;
	contentType = "primitive";
	return 1;
}


// return the size of the removed elements
size_t scene::node::removeElement(size_t index)
{
	if (index != this->index) {
		throw invalid_argument("index need to be equals to the index of the node");
	}
	else {
		this->content = NULL;
	}

	return size;
}

scene::element* scene::node::getElement(size_t index)
{
	if (index != this->index) {
		throw invalid_argument("index need to be equals to the index of the node");
	}
	else {
		//Probleme ici?
		return this;
	}
}


/************************************************************************/
/*							SceneElement	                            */
/************************************************************************/

scene::element::element(size_t index, size_t height) : element{ index, height, 0 } { }

scene::element::element(size_t index, size_t height, size_t size) : index{ index }, size{ size }, height{ height } { }

std::ostream & scene::element::print(std::ostream & os) const
{
	return os << string(this->getHeight() * 4, ' ') << this->getIndex() << " - " << this->getType();
}

/************************************************************************/
/*							scene_iterator                              */
/************************************************************************/

scene::scene_iterator::scene_iterator(const group_ptr & root, size_t index) : root{ root }, rootIndex{ index } {
	// Trouver le premier element
	operator++();
}

void scene::scene_iterator::operator++()
{
	for (rootIndex; rootIndex <= root->getSize(); ++rootIndex) {
		element* elem = root->getElement(rootIndex);
		if (elem->getType() != "group" && elem->getType() != "root") {
			primitive_ptr ptr = (dynamic_cast<node*>(elem))->content;
			if (p != ptr) {
				p = ptr;
				break;
			}
		}
	}
	if (rootIndex > root->getSize()) {
		p = primitive_ptr{ nullptr };
	}
}

/************************************************************************/
/*								TEST                                    */
/************************************************************************/

void test_scene() {

	cout << "test" << endl << endl;
	try {
		scene s{};
		cout << s << endl;

		//TEST ADD

		cout << "addElement(0, true)" << endl;
		s.addElement(0, primitive2d{ }, true);
		cout << s << endl;
		cout << "addElement(0, false)" << endl;
		s.addElement(0, primitive2d{ }, true);
		cout << "addElement(1, true)" << endl;
		s.addElement(1, primitive2d{ }, false);
		cout << s << endl;
		cout << "addElement(2, true)" << endl;
		s.addElement(2, primitive2d{ }, true);
		cout << s << endl;
		cout << "addElement(2, true)" << endl;
		s.addElement(2, primitive2d{ }, true);
		cout << s << endl;
		cout << "addElement(4, true)" << endl;
		s.addElement(4, primitive2d{ }, true);
		cout << s << endl;
		cout << "addElement(1, true)" << endl;
		s.addElement(1, primitive2d{ }, true);
		cout << s << endl;
		cout << "addElement(6, false)" << endl;
		s.addElement(6, primitive2d{ }, false);
		cout << s << endl;

		//TEST REMOVE

		cout << "removeElement(1)" << endl;
		s.removeElement(1);
		cout << s << endl;
		cout << "removeElement(5)" << endl;
		s.removeElement(5);
		cout << s << endl;
		cout << "removeElement(4)" << endl;
		s.removeElement(4);
		cout << s << endl;
		cout << "addElement(3, true)" << endl;
		s.addElement(3, primitive2d{ }, true);
		cout << s << endl;
		cout << "removeElement(3)" << endl;
		s.removeElement(3);
		cout << s << endl;

		int i = 0;
		for (auto& prim : s) {
			i++;
			cout << i;
		}
	}
	catch (exception e) {
		cout << e.what() << endl;
	}

	while (1 == 1);
}
