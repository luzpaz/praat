#ifndef _NUMlapack_h_
#define _NUMlapack_h_
/* NUMlapack.h
 *
 * Copyright (C) 2020 David Weenink
 *
 * This code is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This code is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this work. If not, see <http://www.gnu.org/licenses/>.
 */

#include "melder.h"

#undef max
#undef min

/*
	This interface only works for C++ matrices that have nrow >= ncol!
	The lapack matrix views can only view "contiguous" blocks of a matrix, which means that we
	can only make limited use MATVU arguments of the following NUMlapack functions. 
	either should have rowStride or columnStride larger than 1 but not both!
	A row major layout has rowStride == ncol and columnStrid = 1;
	A column major layout has rowStride = 1 and columnStride == nrow
	The underlying lapack routines always expect colummn major matrix layout (fortran) 
	while in C++ row major matrices are default.
	The matrix 
	1 2
	3 4
	is layed out in memory as 1 2 3 4 (row major, C++) or as 1 3 2 4 (column major, fortran).
	
	The interfaces only works for matrices that have nrow >= ncol!
	
	With some routines we can just input or output standard C++ matrices with row major layout,
	whenever this is not possible the matrix variable name has the extension _CM and you
	really need to input the matrix with physical column major layout!
	(an argument a.transpose() will not work).
	
	As this interface is not yet stable I think that the following option is better:
	Always accept row major layout and in the query part add the extra space 
	needed for the necessary physical transposes.
	In the calculation routine do the actual transposes of the matrices in workspace memory!
	We definitely do don't want to allocate memory in the NUMlapack routines because this would spoil multithreading.
	
*/

int NUMlapack_dgeev_ (const char *jobvl, const char *jobvr, integer *n, double *a, integer *lda, double *wr, double *wi,	double *vl, integer *ldvl, double *vr, integer *ldvr, double *work, integer *lwork, integer *info);
/*  -- LAPACK driver routine (version 3.0) --
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
       Courant Institute, Argonne National Lab, and Rice University
       December 8, 1999

    Purpose
    =======

    NUMlapack_dgeev computes for an N-by-N real nonsymmetric matrix A, the
    eigenvalues and, optionally, the left and/or right eigenvectors.

    The right eigenvector v(j) of A satisfies
                     A * v(j) = lambda(j) * v(j)
    where lambda(j) is its eigenvalue.
    The left eigenvector u(j) of A satisfies
                  u(j)**H * A = lambda(j) * u(j)**H
    where u(j)**H denotes the conjugate transpose of u(j).

    The computed eigenvectors are normalized to have Euclidean norm
    equal to 1 and largest component real.

    Arguments
    =========

    JOBVL   (input) char*
            = 'N': left eigenvectors of A are not computed;
            = 'V': left eigenvectors of A are computed.

    JOBVR   (input) char*
            = 'N': right eigenvectors of A are not computed;
            = 'V': right eigenvectors of A are computed.

    N       (input) long
            The order of the matrix A. N >= 0.

    A       (input/output) double array, dimension (LDA,N)
            On entry, the N-by-N matrix A.
            On exit, A has been overwritten.

    LDA     (input) long
            The leading dimension of the array A.  LDA >= max(1,N).

    WR      (output) double array, dimension (N)
    WI      (output) double array, dimension (N)
            WR and WI contain the real and imaginary parts,
            respectively, of the computed eigenvalues.  Complex
            conjugate pairs of eigenvalues appear consecutively
            with the eigenvalue having the positive imaginary part
            first.

    VL      (output) double array, dimension (LDVL,N)
            If JOBVL = 'V', the left eigenvectors u(j) are stored one
            after another in the columns of VL, in the same order
            as their eigenvalues.
            If JOBVL = 'N', VL is not referenced.
            If the j-th eigenvalue is real, then u(j) = VL(:,j),
            the j-th column of VL.
            If the j-th and (j+1)-st eigenvalues form a complex
            conjugate pair, then u(j) = VL(:,j) + i*VL(:,j+1) and
            u(j+1) = VL(:,j) - i*VL(:,j+1).

    LDVL    (input) long
            The leading dimension of the array VL.  LDVL >= 1; if
            JOBVL = 'V', LDVL >= N.

    VR      (output) double array, dimension (LDVR,N)
            If JOBVR = 'V', the right eigenvectors v(j) are stored one
            after another in the columns of VR, in the same order
            as their eigenvalues.
            If JOBVR = 'N', VR is not referenced.
            If the j-th eigenvalue is real, then v(j) = VR(:,j),
            the j-th column of VR.
            If the j-th and (j+1)-st eigenvalues form a complex
            conjugate pair, then v(j) = VR(:,j) + i*VR(:,j+1) and
            v(j+1) = VR(:,j) - i*VR(:,j+1).

    LDVR    (input) long
            The leading dimension of the array VR.  LDVR >= 1; if
            JOBVR = 'V', LDVR >= N.

    WORK    (workspace/output) double array, dimension (LWORK)
            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.

    LWORK   (input) long
            The dimension of the array WORK.  LWORK >= max(1,3*N), and
            if JOBVL = 'V' or JOBVR = 'V', LWORK >= 4*N.  For good
            performance, LWORK must generally be larger.

            If LWORK = -1, then a workspace query is assumed; the routine
            only calculates the optimal size of the WORK array, returns
            this value as the first entry of the WORK array, and no error
            message related to LWORK is issued by XERBLA.

    INFO    (output) long
            = 0:  successful exit
            < 0:  if INFO = -i, the i-th argument had an illegal value.
            > 0:  if INFO = i, the QR algorithm failed to compute all the
                  eigenvalues, and no eigenvectors have been computed;
                  elements i+1:N of WR and WI contain eigenvalues which
                  have converged.

    =====================================================================
*/

integer NUMlapack_dgesvd_query (constMATVU const& a, constMATVU const& u, constVEC const& singularValues, constMATVU const& vt);

void NUMlapack_dgesvd (constMATVU const& a, MATVU const& inout_u, VEC const& inout_singularValues, MATVU const& inout_vt, VEC const& work);

int NUMlapack_dgesvd_ (const char *jobu, const char *jobvt, integer *m, integer *n, double *a, integer *lda, double *s, double *u, integer *ldu, double *vt, integer *ldvt, double *work,
	integer *lwork, integer *info);/*
     DGESVD computes the singular value decomposition (SVD) of a real
     M-by-N matrix A, optionally computing the left and/or right singular
     vectors. The SVD is written

          A = U * SIGMA * transpose(V)

     where SIGMA is an M-by-N matrix which is zero except for its
     min(m,n) diagonal elements, U is an M-by-M orthogonal matrix, and
     V is an N-by-N orthogonal matrix.  The diagonal elements of SIGMA
     are the singular values of A; they are real and non-negative, and
     are returned in descending order.  The first min(m,n) columns of
     U and V are the left and right singular vectors of A.

     Note that the routine returns V**T, not V.

Parameters
    [in]	JOBU	

              JOBU is CHARACTER*1
              Specifies options for computing all or part of the matrix U:
              = 'A':  all M columns of U are returned in array U:
              = 'S':  the first min(m,n) columns of U (the left singular
                      vectors) are returned in the array U;
              = 'O':  the first min(m,n) columns of U (the left singular
                      vectors) are overwritten on the array A;
              = 'N':  no columns of U (no left singular vectors) are
                      computed.

    [in]	JOBVT	

              JOBVT is CHARACTER*1
              Specifies options for computing all or part of the matrix
              V**T:
              = 'A':  all N rows of V**T are returned in the array VT;
              = 'S':  the first min(m,n) rows of V**T (the right singular
                      vectors) are returned in the array VT;
              = 'O':  the first min(m,n) rows of V**T (the right singular
                      vectors) are overwritten on the array A;
              = 'N':  no rows of V**T (no right singular vectors) are
                      computed.

              JOBVT and JOBU cannot both be 'O'.

    [in]	M	

              M is INTEGER
              The number of rows of the input matrix A.  M >= 0.

    [in]	N	

              N is INTEGER
              The number of columns of the input matrix A.  N >= 0.

    [in,out]	A	

              A is DOUBLE PRECISION array, dimension (LDA,N)
              On entry, the M-by-N matrix A.
              On exit,
              if JOBU = 'O',  A is overwritten with the first min(m,n)
                              columns of U (the left singular vectors,
                              stored columnwise);
              if JOBVT = 'O', A is overwritten with the first min(m,n)
                              rows of V**T (the right singular vectors,
                              stored rowwise);
              if JOBU .ne. 'O' and JOBVT .ne. 'O', the contents of A
                              are destroyed.

    [in]	LDA	

              LDA is INTEGER
              The leading dimension of the array A.  LDA >= max(1,M).

    [out]	S	

              S is DOUBLE PRECISION array, dimension (min(M,N))
              The singular values of A, sorted so that S(i) >= S(i+1).

    [out]	U	

              U is DOUBLE PRECISION array, dimension (LDU,UCOL)
              (LDU,M) if JOBU = 'A' or (LDU,min(M,N)) if JOBU = 'S'.
              If JOBU = 'A', U contains the M-by-M orthogonal matrix U;
              if JOBU = 'S', U contains the first min(m,n) columns of U
              (the left singular vectors, stored columnwise);
              if JOBU = 'N' or 'O', U is not referenced.

    [in]	LDU	

              LDU is INTEGER
              The leading dimension of the array U.  LDU >= 1; if
              JOBU = 'S' or 'A', LDU >= M.

    [out]	VT	

              VT is DOUBLE PRECISION array, dimension (LDVT,N)
              If JOBVT = 'A', VT contains the N-by-N orthogonal matrix
              V**T;
              if JOBVT = 'S', VT contains the first min(m,n) rows of
              V**T (the right singular vectors, stored rowwise);
              if JOBVT = 'N' or 'O', VT is not referenced.

    [in]	LDVT	

              LDVT is INTEGER
              The leading dimension of the array VT.  LDVT >= 1; if
              JOBVT = 'A', LDVT >= N; if JOBVT = 'S', LDVT >= min(M,N).

    [out]	WORK	

              WORK is DOUBLE PRECISION array, dimension (MAX(1,LWORK))
              On exit, if INFO = 0, WORK(1) returns the optimal LWORK;
              if INFO > 0, WORK(2:MIN(M,N)) contains the unconverged
              superdiagonal elements of an upper bidiagonal matrix B
              whose diagonal is in S (not necessarily sorted). B
              satisfies A = U * B * VT, so it has the same singular values
              as A, and singular vectors related by U and VT.

    [in]	LWORK	

              LWORK is INTEGER
              The dimension of the array WORK.
              LWORK >= MAX(1,5*MIN(M,N)) for the paths (see comments inside code):
                 - PATH 1  (M much larger than N, JOBU='N')
                 - PATH 1t (N much larger than M, JOBVT='N')
              LWORK >= MAX(1,3*MIN(M,N) + MAX(M,N),5*MIN(M,N)) for the other paths
              For good performance, LWORK should generally be larger.

              If LWORK = -1, then a workspace query is assumed; the routine
              only calculates the optimal size of the WORK array, returns
              this value as the first entry of the WORK array, and no error
              message related to LWORK is issued by XERBLA.

    [out]	INFO	

              INFO is INTEGER
              = 0:  successful exit.
              < 0:  if INFO = -i, the i-th argument had an illegal value.
              > 0:  if DBDSQR did not converge, INFO specifies how many
                    superdiagonals of an intermediate bidiagonal form B
                    did not converge to zero. See the description of WORK
                    above for details.
*/

int NUMlapack_dggsvd_ (const char *jobu, const char *jobv, const char *jobq, integer *m, integer *n,	integer *p, integer *k, integer *l, double *a, integer *lda, double *b, integer *ldb, double *alpha, double *beta, double *u, integer *ldu, double *v, integer *ldv, double *q, integer *ldq, double *work, integer *iwork, integer *info);
/*  Purpose
    =======

    NUMlapack_dggsvd computes the generalized singular value decomposition (GSVD)
    of an M-by-N real matrix A and P-by-N real matrix B:

        U'*A*Q = D1*( 0 R ),    V'*B*Q = D2*( 0 R )

    where U, V and Q are orthogonal matrices, and Z' is the transpose
    of Z.  Let K+L = the effective numerical rank of the matrix (A',B')',
    then R is a K+L-by-K+L nonsingular upper triangular matrix, D1 and
    D2 are M-by-(K+L) and P-by-(K+L) "diagonal" matrices and of the
    following structures, respectively:

    If M-K-L >= 0,

                        K  L
           D1 =     K ( I  0 )
                    L ( 0  C )
                M-K-L ( 0  0 )

                      K  L
           D2 =   L ( 0  S )
                P-L ( 0  0 )

                    N-K-L  K    L
      ( 0 R ) = K (  0   R11  R12 )
                L (  0    0   R22 )

    where

      C = diag( ALPHA(K+1), ... , ALPHA(K+L) ),
      S = diag( BETA(K+1),  ... , BETA(K+L) ),
      C**2 + S**2 = I.

      R is stored in A(1:K+L,N-K-L+1:N) on exit.

    If M-K-L < 0,

                      K M-K K+L-M
           D1 =   K ( I  0    0   )
                M-K ( 0  C    0   )

                        K M-K K+L-M
           D2 =   M-K ( 0  S    0  )
                K+L-M ( 0  0    I  )
                  P-L ( 0  0    0  )

                       N-K-L  K   M-K  K+L-M
      ( 0 R ) =     K ( 0    R11  R12  R13  )
                  M-K ( 0     0   R22  R23  )
                K+L-M ( 0     0    0   R33  )

    where

      C = diag( ALPHA(K+1), ... , ALPHA(M) ),
      S = diag( BETA(K+1),  ... , BETA(M) ),
      C**2 + S**2 = I.

      (R11 R12 R13 ) is stored in A(1:M, N-K-L+1:N), and R33 is stored
      ( 0  R22 R23 )
      in B(M-K+1:L,N+M-K-L+1:N) on exit.

     The routine computes C, S, R, and optionally the orthogonal
    transformation matrices U, V and Q.

    In particular, if B is an N-by-N nonsingular matrix, then the GSVD of
    A and B implicitly gives the SVD of A*inv(B):
                         A*inv(B) = U*(D1*inv(D2))*V'.
    If ( A',B')' has orthonormal columns, then the GSVD of A and B is
    also equal to the CS decomposition of A and B. Furthermore, the GSVD
    can be used to derive the solution of the eigenvalue problem:
                         A'*A x = lambda* B'*B x.
    In some literature, the GSVD of A and B is presented in the form
                     U'*A*X = ( 0 D1 ),   V'*B*X = ( 0 D2 )
    where U and V are orthogonal and X is nonsingular, D1 and D2 are
    ``diagonal''.  The former GSVD form can be converted to the latter
    form by taking the nonsingular matrix X as

                         X = Q*( I   0    )
                               ( 0 inv(R) ).

    Arguments
    =========

    JOBU    (input) char*
            = 'U':  Orthogonal matrix U is computed;
            = 'N':  U is not computed.

    JOBV    (input) char*
            = 'V':  Orthogonal matrix V is computed;
            = 'N':  V is not computed.

    JOBQ    (input) char*
            = 'Q':  Orthogonal matrix Q is computed;
            = 'N':  Q is not computed.

    M       (input) long
            The number of rows of the matrix A.  M >= 0.

    N       (input) long
            The number of columns of the matrices A and B.  N >= 0.

    P       (input) long
            The number of rows of the matrix B.  P >= 0.

    K       (output) long
    L       (output) long
            On exit, K and L specify the dimension of the subblocks
            described in the Purpose section.
            K + L = effective numerical rank of (A',B')'.

    A       (input/output) double array, dimension (LDA,N)
            On entry, the M-by-N matrix A.
            On exit, A contains the triangular matrix R, or part of R.
            See Purpose for details.

    LDA     (input) long
            The leading dimension of the array A. LDA >= max(1,M).

    B       (input/output) double array, dimension (LDB,N)
            On entry, the P-by-N matrix B.
            On exit, B contains the triangular matrix R if M-K-L < 0.
            See Purpose for details.

    LDB     (input) long
            The leading dimension of the array B. LDA >= max(1,P).

    ALPHA   (output) double array, dimension (N)
    BETA    (output) double array, dimension (N)
            On exit, ALPHA and BETA contain the generalized singular
            value pairs of A and B;
              ALPHA(1:K) = 1,
              BETA(1:K)  = 0,
            and if M-K-L >= 0,
              ALPHA(K+1:K+L) = C,
              BETA(K+1:K+L)  = S,
            or if M-K-L < 0,
              ALPHA(K+1:M)=C, ALPHA(M+1:K+L)=0
              BETA(K+1:M) =S, BETA(M+1:K+L) =1
            and
              ALPHA(K+L+1:N) = 0
              BETA(K+L+1:N)  = 0

    U       (output) double array, dimension (LDU,M)
            If JOBU = 'U', U contains the M-by-M orthogonal matrix U.
            If JOBU = 'N', U is not referenced.

    LDU     (input) long
            The leading dimension of the array U. LDU >= max(1,M) if
            JOBU = 'U'; LDU >= 1 otherwise.

    V       (output) double array, dimension (LDV,P)
            If JOBV = 'V', V contains the P-by-P orthogonal matrix V.
            If JOBV = 'N', V is not referenced.

    LDV     (input) long
            The leading dimension of the array V. LDV >= max(1,P) if
            JOBV = 'V'; LDV >= 1 otherwise.

    Q       (output) double array, dimension (LDQ,N)
            If JOBQ = 'Q', Q contains the N-by-N orthogonal matrix Q.
            If JOBQ = 'N', Q is not referenced.

    LDQ     (input) long
            The leading dimension of the array Q. LDQ >= max(1,N) if
            JOBQ = 'Q'; LDQ >= 1 otherwise.

    WORK    (workspace) double array,
                        dimension (max(3*N,M,P)+N)

    IWORK   (workspace/output) long array, dimension (N)
            On exit, IWORK stores the sorting information. More
            precisely, the following loop will sort ALPHA
               for I = K+1, min(M,K+L)
                   swap ALPHA(I) and ALPHA(IWORK(I))
               endfor
            such that ALPHA(1) >= ALPHA(2) >= ... >= ALPHA(N).

    INFO    (output)long
            = 0:  successful exit
            < 0:  if INFO = -i, the i-th argument had an illegal value.
            > 0:  if INFO = 1, the Jacobi-type procedure failed to
                  converge.  For further details, see subroutine NUMlapack_dtgsja.

    Internal Parameters
    ===================

    TOLA    double
    TOLB    double
            TOLA and TOLB are the thresholds to determine the effective
            rank of (A',B')'. Generally, they are set to
                     TOLA = MAX(M,N)*norm(A)*MAZHEPS,
                     TOLB = MAX(P,N)*norm(B)*MAZHEPS.
            The size of TOLA and TOLB may affect the size of backward
            errors of the decomposition.

    =====================================================================
*/

integer NUMlapack_dhseqr_query (constMATVU const& inout_upperHessenberg_CM, constCOMPVEC const& eigenvalues, constMATVU const& z_CM);
/* Returns the work space size needed */

integer NUMlapack_dhseqr (constMATVU const& inout_upperHessenberg_CM, COMPVEC const& inout_eigenvalues, MATVU const& inout_z_CM, VEC const& work);
/* Returns the number of roots found */

int NUMlapack_dhseqr_ (const char *job, const char *compz, integer *n, integer *ilo, integer *ihi, double *h, integer *ldh, double *wr, double *wi, double *z, integer *ldz, double *work, integer *lwork, integer *info);
/*  -- LAPACK routine (version 3.0) --
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
       Courant Institute, Argonne National Lab, and Rice University
       June 30, 1999


    Purpose
    =======

    NUMlapack_dhseqr computes the eigenvalues of a real upper Hessenberg matrix H
    and, optionally, the matrices T and Z from the Schur decomposition
    H = Z T Z**T, where T is an upper quasi-triangular matrix (the Schur
    form), and Z is the orthogonal matrix of Schur vectors.

    Optionally Z may be postmultiplied into an input orthogonal matrix Q,
    so that this routine can give the Schur factorization of a matrix A
    which has been reduced to the Hessenberg form H by the orthogonal
    matrix Q:  A = Q*H*Q**T = (QZ)*T*(QZ)**T.

    Arguments
    =========

    JOB     (input) char*
            = 'E':  compute eigenvalues only;
            = 'S':  compute eigenvalues and the Schur form T.

    COMPZ   (input) char*
            = 'N':  no Schur vectors are computed;
            = 'I':  Z is initialized to the unit matrix and the matrix Z
                    of Schur vectors of H is returned;
            = 'V':  Z must contain an orthogonal matrix Q on entry, and
                    the product Q*Z is returned.

    N       (input) long
            The order of the matrix H.  N >= 0.

    ILO     (input) long
    IHI     (input) long
            It is assumed that H is already upper triangular in rows
            and columns 1:ILO-1 and IHI+1:N. ILO and IHI are normally
            set by a previous call to NUMlapack_dgebal, and then passed to SGEHRD
            when the matrix output by NUMlapack_dgebal is reduced to Hessenberg
            form. Otherwise ILO and IHI should be set to 1 and N
            respectively.
            1 <= ILO <= IHI <= N, if N > 0; ILO=1 and IHI=0, if N=0.

    H       (input/output) double array, dimension (LDH,N)
            On entry, the upper Hessenberg matrix H.
            On exit, if JOB = 'S', H contains the upper quasi-triangular
            matrix T from the Schur decomposition (the Schur form);
            2-by-2 diagonal blocks (corresponding to complex conjugate
            pairs of eigenvalues) are returned in standard form, with
            H(i,i) = H(i+1,i+1) and H(i+1,i)*H(i,i+1) < 0. If JOB = 'E',
            the contents of H are unspecified on exit.

    LDH     (input) long
            The leading dimension of the array H. LDH >= max(1,N).

    WR      (output) double array, dimension (N)
    WI      (output) double array, dimension (N)
            The real and imaginary parts, respectively, of the computed
            eigenvalues. If two eigenvalues are computed as a complex
            conjugate pair, they are stored in consecutive elements of
            WR and WI, say the i-th and (i+1)th, with WI(i) > 0 and
            WI(i+1) < 0. If JOB = 'S', the eigenvalues are stored in the
            same order as on the diagonal of the Schur form returned in
            H, with WR(i) = H(i,i) and, if H(i:i+1,i:i+1) is a 2-by-2
            diagonal block, WI(i) = sqrt(H(i+1,i)*H(i,i+1)) and
            WI(i+1) = -WI(i).

    Z       (input/output) double array, dimension (LDZ,N)
            If COMPZ = 'N': Z is not referenced.
            If COMPZ = 'I': on entry, Z need not be set, and on exit, Z
            contains the orthogonal matrix Z of the Schur vectors of H.
            If COMPZ = 'V': on entry Z must contain an N-by-N matrix Q,
            which is assumed to be equal to the unit matrix except for
            the submatrix Z(ILO:IHI,ILO:IHI); on exit Z contains Q*Z.
            Normally Q is the orthogonal matrix generated by NUMlapack_dorghr after
            the call to NUMlapack_dgehrd which formed the Hessenberg matrix H.

    LDZ     (input) long
            The leading dimension of the array Z.
            LDZ >= max(1,N) if COMPZ = 'I' or 'V'; LDZ >= 1 otherwise.

    WORK    (workspace/output) double array, dimension (LWORK)
            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.

    LWORK   (input) long
            The dimension of the array WORK.  LWORK >= max(1,N).

            If LWORK = -1, then a workspace query is assumed; the routine
            only calculates the optimal size of the WORK array, returns
            this value as the first entry of the WORK array, and no error
            message related to LWORK is issued by XERBLA.

    INFO    (output) long
            = 0:  successful exit
            < 0:  if INFO = -i, the i-th argument had an illegal value
            > 0:  if INFO = i, NUMlapack_dhseqr failed to compute all of the
                  eigenvalues in a total of 30*(IHI-ILO+1) iterations;
                  elements 1:ilo-1 and i+1:n of WR and WI contain those
                  eigenvalues which have been successfully computed.

    =====================================================================
*/

int NUMlapack_dpotf2_ (const char *uplo, integer *n, double *a, integer *lda, integer *info);
/*  -- LAPACK routine (version 3.0) --
       Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd.,
       Courant Institute, Argonne National Lab, and Rice University
       February 29, 1992


    Purpose
    =======

    NUMlapack_dpotf2 computes the Cholesky factorization of a real symmetric
    positive definite matrix A.

    The factorization has the form
       A = U' * U ,  if UPLO = 'U', or
       A = L  * L',  if UPLO = 'L',
    where U is an upper triangular matrix and L is lower triangular.

    This is the unblocked version of the algorithm, calling Level 2 BLAS.

    Arguments
    =========

    UPLO    (input) char *
            Specifies whether the upper or lower triangular part of the
            symmetric matrix A is stored.
            = 'U':  Upper triangular
            = 'L':  Lower triangular

    N       (input) long
            The order of the matrix A.  N >= 0.

    A       (input/output) double array, dimension (LDA,N)
            On entry, the symmetric matrix A.  If UPLO = 'U', the leading
            n by n upper triangular part of A contains the upper
            triangular part of the matrix A, and the strictly lower
            triangular part of A is not referenced.  If UPLO = 'L', the
            leading n by n lower triangular part of A contains the lower
            triangular part of the matrix A, and the strictly upper
            triangular part of A is not referenced.

            On exit, if INFO = 0, the factor U or L from the Cholesky
            factorization A = U'*U  or A = L*L'.

    LDA     (input) long
            The leading dimension of the array A.  LDA >= max(1,N).

    INFO    (output) long
            = 0: successful exit
            < 0: if INFO = -k, the k-th argument had an illegal value
            > 0: if INFO = k, the leading minor of order k is not
                 positive definite, and the factorization could not be
                 completed.

*/

int NUMlapack_dsyev_ (const char *jobz, const char *uplo, integer *n, double *a,	integer *lda,
	double *w, double *work, integer *lwork, integer *info);
/* Purpose =======

	NUMlapack_dsyev computes all eigenvalues and, optionally, eigenvectors of a
	real symmetric matrix A.

    Arguments
    =========

    JOBZ    (input) char*
            = 'N':  Compute eigenvalues only;
            = 'V':  Compute eigenvalues and eigenvectors.

    UPLO    (input) char*
            = 'U':  Upper triangle of A is stored;
            = 'L':  Lower triangle of A is stored.

    N       (input) long
            The order of the matrix A.  N >= 0.

    A       (input/output) double array, dimension (LDA, N)
            On entry, the symmetric matrix A.  If UPLO = 'U', the
            leading N-by-N upper triangular part of A contains the
            upper triangular part of the matrix A.  If UPLO = 'L',
            the leading N-by-N lower triangular part of A contains
            the lower triangular part of the matrix A.
            On exit, if JOBZ = 'V', then if INFO = 0, A contains the
            orthonormal eigenvectors of the matrix A.
            If JOBZ = 'N', then on exit the lower triangle (if UPLO='L')
            or the upper triangle (if UPLO='U') of A, including the
            diagonal, is destroyed.

    LDA     (input) long
            The leading dimension of the array A.  LDA >= max(1,N).

    W       (output) double array, dimension (N)
            If INFO = 0, the eigenvalues in ascending order.

    WORK    (workspace/output) double array, dimension (LWORK)
            On exit, if INFO = 0, WORK(1) returns the optimal LWORK.

    LWORK   (input) long
            The length of the array WORK.  LWORK >= max(1,3*N-1).
            For optimal efficiency, LWORK >= (NB+2)*N,
            where NB is the blocksize for NUMlapack_dsytrd returned by NUMlapack_ilaenv.

            If LWORK = -1, then a workspace query is assumed; the routine
            only calculates the optimal size of the WORK array, returns
            this value as the first entry of the WORK array, and no error
            message related to LWORK is issued by XERBLA.

    INFO    (output) long
            = 0:  successful exit
            < 0:  if INFO = -i, the i-th argument had an illegal value
            > 0:  if INFO = i, the algorithm failed to converge; i
                  off-diagonal elements of an intermediate tridiagonal
                  form did not converge to zero.

	=====================================================================
*/


int NUMlapack_dtrtri_ (const char *uplo, const char *diag, integer *n, double *
	a, integer *lda, integer *info);
/*  Purpose
    =======

    NUMlapack_dtrtri computes the inverse of a real upper or lower triangular
    matrix A.

    This is the Level 3 BLAS version of the algorithm.

    Arguments
    =========

    UPLO    (input) char*
            = 'U':  A is upper triangular;
            = 'L':  A is lower triangular.

    DIAG    (input) char*
            = 'N':  A is non-unit triangular;
            = 'U':  A is unit triangular.

    N       (input) long
            The order of the matrix A.  N >= 0.

    A       (input/output) double array, dimension (LDA,N)
            On entry, the triangular matrix A.  If UPLO = 'U', the
            leading N-by-N upper triangular part of the array A contains
            the upper triangular matrix, and the strictly lower
            triangular part of A is not referenced.  If UPLO = 'L', the
            leading N-by-N lower triangular part of the array A contains
            the lower triangular matrix, and the strictly upper
            triangular part of A is not referenced.  If DIAG = 'U', the
            diagonal elements of A are also not referenced and are
            assumed to be 1.
            On exit, the (triangular) inverse of the original matrix, in
            the same storage format.

    LDA     (input) long
            The leading dimension of the array A.  LDA >= max(1,N).

    INFO    (output) long
            = 0: successful exit
            < 0: if INFO = -i, the i-th argument had an illegal value
            > 0: if INFO = i, A(i,i) is exactly zero.  The triangular
                 matrix is singular and its inverse can not be computed.

    =====================================================================
*/

int NUMlapack_dtrti2_ (const char *uplo, const char *diag, integer *n, double *a, integer *lda, integer *info);
/*  Purpose
    =======

    NUMlapack_dtrti2 computes the inverse of a real upper or lower triangular
    matrix.

    This is the Level 2 BLAS version of the algorithm.

    Arguments
    =========

    UPLO    (input) char*
            Specifies whether the matrix A is upper or lower triangular.
            = 'U':  Upper triangular
            = 'L':  Lower triangular

    DIAG    (input) char*
            Specifies whether or not the matrix A is unit triangular.
            = 'N':  Non-unit triangular
            = 'U':  Unit triangular

    N       (input) long
            The order of the matrix A.  N >= 0.

    A       (input/output) double array, dimension (LDA,N)
            On entry, the triangular matrix A.  If UPLO = 'U', the
            leading n by n upper triangular part of the array A contains
            the upper triangular matrix, and the strictly lower
            triangular part of A is not referenced.  If UPLO = 'L', the
            leading n by n lower triangular part of the array A contains
            the lower triangular matrix, and the strictly upper
            triangular part of A is not referenced.  If DIAG = 'U', the
            diagonal elements of A are also not referenced and are
            assumed to be 1.

            On exit, the (triangular) inverse of the original matrix, in
            the same storage format.

    LDA     (input) long
            The leading dimension of the array A.  LDA >= max(1,N).

    INFO    (output) long
            = 0: successful exit
            < 0: if INFO = -k, the k-th argument had an illegal value

    =====================================================================
*/

#endif /*_NUMlapack_h_ */