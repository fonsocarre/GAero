module precisionMod
    use, intrinsic :: iso_c_binding
	implicit none


    ! Public Declarations:
    public

    ! Public Parameters:

    ! Kinds for specified integer ranges.
    integer, parameter :: is = c_int

    ! System default real kinds.
    integer, parameter :: rs = c_float    ! real single precision
    integer, parameter :: rd = c_double  ! real double precision

    ! Kind for working real precision.
    integer, parameter :: ip = is
    integer, parameter :: wp = rd

end module precisionMod
