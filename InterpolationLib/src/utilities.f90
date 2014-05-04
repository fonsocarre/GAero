!
!   utilities.f90
!   InterpolationLib
!
!   Created by Alfonso Carre on 04/05/14.
!   Copyright 2014 Alfonso Carre. All rights reserved.
!
module utilities
    implicit none

    public

contains

subroutine reporterror(error)
    character(len=*), intent(IN) :: error

    write(*,*) 'ERROR: ', error
    stop

end subroutine reporterror


end module utilities