module InterpolationKernel
    use precisionMod
    !use utilities
    use mkl95_lapack
    use mkl95_blas
    implicit none
    
    character(len=*), parameter        :: MODname = 'InterpolationKernel'
    
    private
    public :: interpolationOrder0
    
contains

    !**************************************************************
    ! INTERPOLATIONORDER0
    !**************************************************************
    ! Dado un array de puntos "fijos" (S), interpola mediante un
    !   spline de orden 0 en los puntos "móviles" (A) las
    !   variables h, que deben ser una matriz con una columna
    !   para cada variable a interpolar
    !
    !   La RBF es la función dada por el argumento RBF
    !
    !   Es necesario linkar LAPACK y BLAS, ambas con la interfaz
    !       de Fortran95
    !--------------------------------------------------------------
    subroutine interpolationOrder0(coorS, coorA, hS, hA, RBF) bind(c)
        real(wp), intent(IN)        :: coorS(:,:)
        real(wp), intent(IN)        :: coorA(:,:)
        real(wp), intent(IN)        :: hS(:,:)
        character(c_char), intent(IN):: RBF
        real(wp), intent(OUT)       :: hA(:,:)
        
        real(wp), pointer           :: Css(:,:)
        real(wp), pointer           :: Aas(:,:)
        real(wp), pointer           :: Ch(:,:)

        real(wp), allocatable       :: hStemp(:,:)
        
        integer(ip)                 :: i
        integer(ip)                 :: ia
        integer(ip)                 :: info
        integer(ip)                 :: iPiv(size(coorS(:,1)) + 1)
        
        integer(ip)                 :: Ns
        integer(ip)                 :: Na
        integer(ip)                 :: nCol
        integer(ip)                 :: iCol
        
        real(wp), allocatable       :: temp(:)

        Ns = size(coorS(:,1))
        Na = size(coorA(:,1))
        nCol = size(hS(1,:))

        call createCss(coorS, Ns+1, Css)

        ! LAPACK CALL ******************************************************
        info = 0
        iPiv = 0
        call getrf(a=Css, ipiv=iPiv, info=info)
        if (info /= 0) call reportError(MODname//'LAPACK GETRF ERROR ')

        info = 0
        call getri(Css, iPiv)
        if (info /= 0) call reportError(MODname//'LAPACK GETRI ERROR ')
        !*******************************************************************

        allocate(hStemp(Ns+1, nCol))
        allocate(Ch(Ns+1, nCol))


        hStemp(1,:) = 0.0_wp
        hStemp(2:,:) = hS
        call gemm(Css, hStemp, Ch)

        do ia=1, Na
            call gemm(createAas(coorS, coorA, Na, Ns, ia), Ch, hA(ia:ia,:))
        end do

        deallocate(Css)
        deallocate(Ch)
    end subroutine interpolationOrder0
    
    subroutine createCss(coorS, N, CssOut, RBF, rho)
        real(wp), intent(IN)        :: coorS(:,:)
        integer(ip), intent(IN)     :: N
        character(c_char), intent(IN):: RBF
        real(wp), intent(IN)        :: rho
        real(wp), pointer           :: CssOut(:,:)
        
        integer(ip)                 :: i
        integer(ip)                 :: j
        integer(ip)                 :: Ns
        
        Ns = size(coorS(:,1))
        
        allocate(CssOut(Ns+1,Ns+1))

        CssOut(:,1) = 1.0_wp
        CssOut(1,:) = 1.0_wp
        CssOut(1,1) = 0.0_wp
        
        do i=1,Ns
            do j=1,i
                CssOut(i+1,j+1) = d(coorS(i,:), coorS(j,:))
                CssOut(j+1,i+1) = CssOut(i+1,j+1)
            end do
        end do
    end subroutine createCss
    
    function createAas(coorS, coorA, Na, Ns, ia) result(Aas)
        real(wp), intent(IN)        :: coorS(:,:)
        real(wp), intent(IN)        :: coorA(:,:)
        real(wp)                    :: Aas(1, Ns+1)
        integer(ip), intent(IN)     :: Na
        integer(ip), intent(IN)     :: Ns
        integer(ip), intent(IN)     :: ia

        integer(ip)                 :: j


        Aas(1,1) = 1.0_wp

        do j=1,Ns
            Aas(1,j+1) = d(coorA(ia,:), coorS(j,:))
        end do
    end function createAas
    
    pure function d(v1,v2)
        real(wp), intent(IN)        :: v1(:)
        real(wp), intent(IN)        :: v2(:)
        real(wp)                    :: d
        
        d = sqrt(dot_product(v1-v2, v1-v2))
    end function d

    pure function wendlandC2(v1,v2, rho) result(wC2)
        real(wp), intent(IN)        :: v1(:)
        real(wp), intent(IN)        :: v2(:)
        real(wp), intent(IN)        :: rho
        real(wp)                    :: wC2

        real(wp)                    :: distance

        distance = d(v1,v2)

        wC2 = ((1.0_wp - distance/rho)**4)*(4.0_wp*distance + 1._wp)
    end function wendlandC2

end module InterpolationKernel