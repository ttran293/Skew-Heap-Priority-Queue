// CMSC 341 - Spring 2020 - Project 4
// JQueue: a job queue using a skew heap and function pointers

#include "jqueue.h"



using std::cout;
using std::endl;
using std::domain_error;

JQueue::JQueue(prifn_t priFn) {
    _size=0;
    _heap= nullptr;
    priority=priFn;
    count=0;
}

//destructor helper
void JQueue::desHelp(Node* node)
{
    if (node == nullptr) return;
    else {
        if (node->_right!= nullptr) desHelp(node->_right);
        if (node->_left!= nullptr) desHelp(node->_left);
        delete node;
    }
}
JQueue::~JQueue() {desHelp(_heap);}

//copy constructor helper
Node* JQueue::ccHelp(Node* rhs)
{
    if (rhs== nullptr) return nullptr;

    Node *copy = new Node(rhs->_job);
    copy->_left= ccHelp(rhs->_left);
    copy->_right=ccHelp(rhs->_right);

    return copy;
}
JQueue::JQueue(const JQueue& rhs) {
    _size=rhs._size;
    priority=rhs.priority;
    count = 0;
    if(rhs._heap== nullptr)
        _heap= nullptr;
    else
        _heap = ccHelp(rhs._heap);
}

//self assigncheck
//cchelp
JQueue& JQueue::operator=(const JQueue& rhs) {


    if (this!=&rhs)
    {
        //clear
        desHelp(_heap);

        _size = rhs._size;
        priority = rhs.priority;
        count = 0;

        if (rhs._heap == nullptr) _heap = nullptr;
        else _heap = ccHelp(rhs._heap);
    }
    return *this;
}



void JQueue::insertJob(const job_t& input) {

    //if heap is empty make new node = heap
    if (_heap == nullptr) {
        _heap = new Node(input);
        _size++;
    } else {//use mergehelp to insert
     Node* _newNode = new Node(input);
    _heap = mergeHelp(_heap, _newNode);
    _size++;//updatesize
    }

}
//return root.data
job_t JQueue::getNextJob() {

    if (_heap != nullptr){
    Node *old =  _heap;
    job_t extract = old->_job;//store job
    _heap = mergeHelp(_heap->_left,_heap->_right);
    delete old;//delete root
    _size--;//update size
    return extract;
    }
    else throw domain_error("Wrong");
}


Node* JQueue::mergeHelp(Node* h1, Node* h2)
{
    //If p1 is Null, return p2; similarly, if p2 is Null, return p1.
    if (h1 == nullptr)
        return h2;
    if (h2 == nullptr)
        return h1;
    //Make sure pri1 has higher priority
    int pri1 = priority (h1->_job);
    int pri2 = priority (h2->_job);
    if (pri1 < pri2)
        std::swap(h1,h2);
    //Swap the left and right subtrees of p1.
    std::swap(h1->_left,h1->_right);
    //Recursively merge p2 and the left subtree of p1,
    //replacing the left subtree of p1 with the result of the recursive merge
    h1->_left = mergeHelp(h2, h1->_left);
    return h1;
}
void JQueue::mergeHelp2(Node* node)
{
    if (node== nullptr)
        return;
    mergeHelp2(node->_right);
    mergeHelp2(node->_left);

    Node *copy = new Node(node->_job);
    _heap = mergeHelp(_heap,copy);
}
void JQueue::mergeWithQueue(JQueue& rhs) {

    //ignore if rhs is itself
    if (this!=&rhs)
    {
        //if same priority fnc, allow merge
        if (this->priority ==rhs.priority)
        {
         mergeHelp2(rhs._heap);
         _size+=rhs._size;
         desHelp(rhs._heap);//clear rhs
         rhs._heap= nullptr;
        }
        else throw domain_error("wrong");
    }
}

//clear function use deshelp
//setevery thing to default value
void JQueue::clear() {
    desHelp(_heap);
    priority= nullptr;
    _size=0;
    count=0;
}

//count node function
int JQueue::size(Node* node) const {
    if(node== nullptr)
        return 0;
    else
        return size(node->_left)+size(node->_right)+1;
}
//Return the current number of jobs in the queue.
int JQueue::numJobs() const {
    return _size;
}

//preorder traversal
//help print jqueue
void JQueue::preOrder(Node* node) const {
    if (node== nullptr)
        return;

    cout<<"[" <<  priority(node->_job) << "] " << node->_job<<endl;

    preOrder(node->_left);
    preOrder(node->_right);

}
void JQueue::printJobQueue() const {
    if (_heap == nullptr) cout<<"Heap is empty"<<endl;
    else  preOrder(_heap);
}

//return priority of this jq
prifn_t JQueue::getPriorityFn() const {
    return priority;
}

//store skewheap into an array
//array hold job
void JQueue::treeTOarr(Node* node, job_t arr[])
{
    if (node== nullptr)
        return;

    //in order traversal
    treeTOarr(node->_left, arr);

    //store each job into array
    arr[count] = node->_job;
    count++;//update count

    treeTOarr(node->_right,arr);
}

//rebuild function with new priority
//_newPrio = new priority function
//_oldSize
Node * JQueue::rebuild(prifn_t _newPrio, int _oldSize)
{
    //initialize count and array size
    count = 0;
    job_t arr[_oldSize];

    //store job_t into array
    treeTOarr(_heap, arr);

    //detele old heap
    desHelp(_heap);
    _heap = nullptr;

    //set up new heap with new priority
    this->priority=_newPrio;
    this->_size = 0;

    //insert each job into skew heap
    for (int i = 0; i < _oldSize; ++i) {
       this-> insertJob(arr[i]);//insert data stored from array to new heap
    }

    return _heap;
}
//Set a new priority function. Must rebuild the heap!
void JQueue::setPriorityFn(prifn_t priFn) {

    //if priority is the same function do nothing
    if (priority!=priFn)
        rebuild(priFn,_size);

}
//   ((((181)316)323)323)
//   ((((120)120)300)1800)
//dump helper function inorder traversal
void JQueue::dumpHelp(Node* node) const {
    if (node== nullptr)
        return;
    cout<<"(";
    dumpHelp(node->_left);
    cout<< priority(node->_job) ;
    dumpHelp(node->_right);
    cout<<")";

}
void JQueue::dump() const {
    if (_heap == nullptr)
        cout<<"Heap is empty"<<endl;
    else{
        dumpHelp(_heap);
        cout<<endl;
    }
}



ostream& operator<<(ostream& sout, const job_t& job) {
  sout << "Job: " << job._name << ", pri: " << job._priority
       << ", u: " << job._user << ", g: " << job._group << ", proc: "
       << job._proc << ", mem: " << job._mem << ", time: " << job._time;
  return sout;
}

ostream& operator<<(ostream& sout, const Node& node) {
  sout << node._job;
  return sout;
}

