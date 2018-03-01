#include <vector>
#include <cstdio>

template <class T> class list{
	public:
		//////////////////////////////
		// Standard list operations //
		//////////////////////////////

		void insert(T element, int priority);
		bool del(T element);
		//concatenate();
		//split();
		//findPrevious();
		//findNext();
		//search();

		///////////////////////////////
		// Priority queue operations //
		///////////////////////////////
		
		bool updatePriority(T element, int priority);
		T findMin();

	private:
		struct S {
			T element;
			int priority;
		};
		std::vector<S> vec;
		size_t findIndex(T element);
};

template<class T>
bool list<T>::updatePriority(T element, int priority){
	size_t index = findIndex(element);
	if(index==-1) return false;
	list::vec[index].priority = priority;
	return true;
}

template<class T>
size_t list<T>::findIndex(T element){
	for(size_t i=0;i<list::vec.size();i++){
		if(list::vec[i].element==element) return i;
	}
	return -1;
}

template<class T>
void list<T>::insert(T element, int priority){
	S s;
	s.element = element;
	s.priority = priority;

	list::vec.push_back(s);
}

template<class T>
bool list<T>::del(T element){
	size_t index = list::findIndex(element);	
	if(index==-1) return false;
	list::vec.erase(list::vec.begin()+index);
	return true;
}

template<class T>
T list<T>::findMin(){

	T minSoFar = list::vec[0].element;
	int minSoFarPriority = list::vec[0].priority;

	for(int i = 1 ; i < list::vec.size() ; i++ ){
		if(vec[i].priority < minSoFarPriority ){
			minSoFar = vec[i].element;
		}
	}
	return minSoFar;
}
