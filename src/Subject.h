#include <iostream>
#include <algorithm>
#include <vector>
#include <memory>
#include <string>

class SubjectBase;

class ObserverBase
{
public:
    virtual ~ObserverBase() = default;
    virtual void update() = 0;
    virtual void subscribe(std::shared_ptr<SubjectBase> subject) = 0;
    virtual void unsubscribe() = 0;
};

class SubjectBase
{
public:
    virtual ~SubjectBase() = default;
    virtual void attach(std::shared_ptr<ObserverBase> observer) = 0;
    virtual void detach(std::shared_ptr<ObserverBase> observer) = 0;
    virtual void notify() = 0;
};

class Subject : public SubjectBase
{
public:
    ~Subject() override = default;
    void attach(std::shared_ptr<ObserverBase> observer) override
    {
        auto it = std::find(mObservers.begin(), mObservers.end(), observer);
        if(it == mObservers.end())
        {
            mObservers.push_back(observer);
        }
        else
        {
            std::cout << "(Subject) Attach again, ignored.\n";
        }
    }
    void detach(std::shared_ptr<ObserverBase> observer) override
    {
        auto it = std::find(mObservers.begin(), mObservers.end(), observer);
        while(it != mObservers.end())
        {
            mObservers.erase(it);
            it = std::find(mObservers.begin(), mObservers.end(), observer);
        }
    }
    void notify() override
    {
        for(auto& i : mObservers)
        {
            i->update();
        }
    }

private:
    std::vector<std::shared_ptr<ObserverBase>> mObservers;
};

class Observer : public ObserverBase, public std::enable_shared_from_this<Observer>
{
public:
    Observer() = default;

    ~Observer() override = default;

    void subscribe(std::shared_ptr<SubjectBase> s) override
    {
        if(auto subject = mSubject.lock())
        {
            std::cout << "(Observer\n";
        }
        else
        {
            mSubject = s;
            s->attach(shared_from_this());
        }
    }
    void update() override { std::cout << "(Observe\n"; }

    void unsubscribe() override
    {
        if(auto subject = mSubject.lock())
        {
            subject->detach(shared_from_this());
            std::cout << "(Observer  Detached.\n";
            mSubject.reset();
        }
        else
        {
            std::cout << "(Observer) Unable to detach or already detached.\n";
        }
    }

private:
    std::weak_ptr<SubjectBase> mSubject;
};