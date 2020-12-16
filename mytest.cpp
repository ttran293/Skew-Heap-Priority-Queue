// CMSC 341 - Spring 2020 - Project 4
// student driver test
// I took same function of
#include "jqueue.h"
using namespace std;

int priorityFn1(const job_t &job);
int priorityFn2(const job_t &job);

int main() {

    // Create a job queue with capacity five and priority function
    // priorityFn1 same function in driver.cpp

    JQueue jobQueue(priorityFn1);

    // Create some test jobs and insert them into the queue
    job_t testJob1("A", 50, 3, 11, 16 , 1024, 120 );
    job_t testJob2("B", 50, 7, 5,  128, 512 , 300 );
    job_t testJob3("C", 20, 3, 0,  128, 1024, 1800);
    job_t testJob4("D", 50, 3, 11, 16 , 1024, 120 );


    jobQueue.insertJob(testJob3);
    jobQueue.insertJob(testJob1);
    jobQueue.insertJob(testJob2);
    jobQueue.insertJob(testJob4);


    // Print the queue, change priority function, print again, change
    // again, print again.  Changing the priority function should cause
    // the heap to be rebuilt.
    std::cout << std::endl;
    jobQueue.printJobQueue();
    std::cout << "Dump the queue:\n";
    jobQueue.dump();

    jobQueue.setPriorityFn(priorityFn2);

    std::cout << std::endl;
    jobQueue.printJobQueue();
    std::cout << "Dump the queue:\n";
    jobQueue.dump();

    jobQueue.setPriorityFn(priorityFn1);

    std::cout << std::endl;
    jobQueue.printJobQueue();
    std::cout << "Dump the queue:\n";
    jobQueue.dump();

   //  Remove and print jobs in priority order
    std::cout << "\nFinal queue in priority order:\n";
    while (jobQueue.numJobs() > 0) {
        job_t job = jobQueue.getNextJob();
        std::cout << "[" << priorityFn1(job) << "] " << job << std::endl;
    }

    //Should throw a domain_error exception if the queue is empty when the function is called.
    try { jobQueue.getNextJob(); }
    catch (std::exception& e) { std::cout << "This should throws domain error since jobQueue is empty" << endl; }


    cout << "\n";
    //Create second JQueue with same priority fn2
    JQueue jobQueue2(priorityFn2);
    jobQueue2.insertJob(testJob3);
    jobQueue2.insertJob(testJob1);
    jobQueue2.insertJob(testJob2);
    jobQueue2.insertJob(testJob4);

    jobQueue2.printJobQueue();
    jobQueue2.dump();

    //Test self merge results in duplicate keys
    cout << "Test self merge. JQ2 should be the same\n";
    jobQueue2.mergeWithQueue(jobQueue2);
    jobQueue2.dump();
    cout<<"\n";


    JQueue jobQueue3(priorityFn1);
    job_t a1("A1", 51, 3, 11, 16 , 1024, 120 );
    job_t b1("B1", 52, 7, 5,  128, 512 , 300 );
    job_t c1("C1", 23, 3, 0,  128, 1024, 1800);
    job_t d1("D1", 54, 3, 11, 16 , 1024, 120 );
    jobQueue3.insertJob(a1);
    jobQueue3.insertJob(b1);
    jobQueue3.insertJob(c1);
    jobQueue3.insertJob(d1);


    JQueue jobQueue4(priorityFn1);
    job_t a2("A2", 10, 3, 11, 16 , 1024, 120 );
    job_t b2("B2", 20, 7, 5,  128, 512 , 300 );
    job_t c2("C2", 30, 3, 0,  128, 1024, 1800);
    job_t d2("D2", 40, 3, 11, 16 , 1024, 120 );
    jobQueue4.insertJob(a2);
    jobQueue4.insertJob(b2);
    jobQueue4.insertJob(c2);
    jobQueue4.insertJob(d2);

    cout<<"Job Queue 3"<<endl;
    jobQueue3.dump();
    jobQueue3.printJobQueue();

    cout<<"\nJob Queue 4"<<endl;
    jobQueue4.dump();
    jobQueue4.printJobQueue();

    jobQueue3.mergeWithQueue(jobQueue4);

    cout<<"After Merge 3 and 4. JQ3 should have size of 8. JQ4 should be empty"<<endl;
    cout<<"Job Queue 3 Size = "<< jobQueue3.numJobs()<<endl;
    jobQueue3.dump();
    jobQueue3.printJobQueue();

    cout<<"\nJob Queue 4"<<endl;
    jobQueue4.dump();
    jobQueue4.printJobQueue();
    try { jobQueue4.getNextJob(); }
    catch (std::exception& e) {
       std::cout << "This should throws domain error since jobQueue 4 is empty" << endl;}



//    //Test copy constructor
    JQueue copy(jobQueue2);

    cout << "\nTest copy constructor: \n";
    copy.printJobQueue();

    cout << "Change JQ2, the Copy should stay the same: \n";
    jobQueue2.getNextJob();
    cout << "Size of Original: " << jobQueue2.numJobs()<<endl;
    cout<<"Original:";jobQueue2.dump();
    cout<< "Size of Copy CC: " << copy.numJobs() << endl;
    cout<<"Copy:";copy.dump();

    cout<<"\nTest assignment operator"<<endl;
    JQueue asop = jobQueue2;
    asop.printJobQueue();

    cout << "Change JQ2, the AOP should stay the same: \n";
    jobQueue2.getNextJob();
    cout << "Size of Original: " << jobQueue2.numJobs() <<endl;
    cout<<"Original:";jobQueue2.dump();

    cout<< "Size of Copy AOP: " << asop.numJobs() << endl;
    cout<<"Copy:";asop.dump();


    //check if priorty the same fucntion then do nothing
    jobQueue2.setPriorityFn(priorityFn2);
    cout<<"Size:"<<jobQueue2.numJobs()<<endl;
    jobQueue2.dump();


    return 0;


}

int priorityFn1(const job_t &job) {
        float pri;


        pri = 200.0 + job._priority;

        // If special user (7 or 23) add 200
        if (job._user == 7 || job._user == 23) pri += 200;

        // If special group (0 or 11) add 100
        if (job._group == 0 || job._group == 11) pri += 100;

        // Subtract weighted proc, mem, and time
        //   1.0 * proc, 0.01 * mem, (1/3600) * time
        pri = pri - job._proc - 0.01 * job._mem - (1.0 / 3600.0) * job._time;

        return int(pri+123);
    }

    int priorityFn2(const job_t &job) {
        return int(job._time + job._group);
    }


