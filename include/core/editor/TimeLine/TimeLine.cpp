#include "TimeLine.hpp"

namespace PDJE_TIMELINE{
    
    bool
    TimeLine::Save()
    {
        try{
        if(!mark.Activate()){
            return false;
        }
        return mark.Append();
        }
        catch(const std::exception& e){
            critlog("failed to save. error occurred. What: ");
            critlog(e.what());
            return false;
        }
    }
    bool
    TimeLine::Undo()
    {
        try{
        if(mark.MoveBack()){
            return mark.Activate();
        }else{
            return false;
        }
        }
        catch(const std::exception& e){
            critlog("failed to undo. error occurred. What: ");
            critlog(e.what());
            return false;
        }
    }

    bool
    TimeLine::Redo()
    {
        try{
            if(mark.MoveFront()){
                return mark.Activate();
            }else{
                return false;//not error
            }
        }
        catch(const std::exception& e){
            critlog("failed to redo. error occurred. What: ");
            critlog(e.what());
            return false;
        }
    }

    bool
    TimeLine::Go(const std::string& OID)
    {
        try
        {
            if(mark.Move(OID)){
                return mark.Activate();
            }else{
                return false;
            }
        }
        catch(const std::exception& e)
        {
            critlog("failed to Go. error occurred. What: ");
            critlog(e.what());
            return false;
        }
    }

    void
    TimeLine::Diff(const OID& origin, const OID& compare)
    {
        NOIMPL CRASH // todo- impl
    }

    
};