#pragma once
#define collisions_h

#include "shapes.h"

template<class Ta, class Tb>
inline bool Collided_Rect(const Ta& A, const Tb& B) noexcept
{
	return (
		( (A.Right() >= B.Left() && A.Right()  <=  B.Right())  || (A.Left() <= B.Left()   && A.Left() >= B.Right()) )   && 
		( (A.Bottom() >= B.Top()  && A.Bottom() <=  B.Bottom()) || (A.Top()  <= B.Bottom() && A.Top()  >= B.Top()) ) 
		);  

}
