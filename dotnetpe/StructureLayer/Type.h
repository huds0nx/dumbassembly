#pragma once

namespace dotnetpe
{

    class Type
    {
    public:
        virtual bool                IsTypeDef       () const = 0;
        virtual bool                IsTypeRef       () const = 0;

        virtual const char*         GetName         () const = 0;
        virtual const char*         GetNamespace    () const = 0;

        virtual bool                operator==      ( const Type& other ) const = 0;

        bool                        operator!=      ( const Type& other ) const
        {
            return !(*this == other );
        }
    };

}
