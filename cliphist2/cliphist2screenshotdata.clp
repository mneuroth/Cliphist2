   
CLIPHIST2    VERSION          < T h i s   i s   a   c l i p b o a r d   t e x t   e n t r y   � c o n f i g u r e   - s t a t i c   - d e b u g - a n d - r e l e a s e   - o p e n s o u r c e   - p l a t f o r m   w i n 3 2 - g + +   - q t - l i b j p e g   < T h i s   i s   a   c l i p b o a r d   t e x t   e n t r y   L T h i s   w a s   t h e   l a s t   c l i p b o a r d   t e x t   e n t r y    c l i p h i s t 2   f a S e t t i n g s . s e t V a l u e ( " A p p / D a t a F i l e N a m e " , m _ s F i l e N a m e ) ;     C l i p h i s t W i n d o w : :  � v o i d   C l i p h i s t W i n d o w : : O n A b o u t ( ) 
 { 
         Q M e s s a g e B o x : : a b o u t ( t h i s , t r ( " A b o u t   A p p l i c a t i o n " ) , Q S t r i n g ( t r ( " < b > % 1 < / b > < s m a l l > < p > V e r s i o n   % 2   f r o m   % 3 < / p > < p > ( c )   2 0 1 0   b y   % 4 < / p > L i c e n s e :   % 5 < / p > < s m a l l > " ) ) . a r g ( T I T L E , V E R S I O N , _ _ D A T E _ _ , A U T H O R S , L I C E N S E ) ) ; 
 } 
  n v o i d   C l i p h i s t W i n d o w : : O n H e l p ( ) 
 { 
         Q M e s s a g e B o x : : i n f o r m a t i o n ( t h i s , t r ( " H e l p " ) , Q S t r i n g ( t r ( " < p > S o r r y ,   n o   h e l p   a v a i l a b l e   y e t   ! < / p > < p > S e e   % 1   f o r   m o r e   i n f o r m a t i o n . < / p > " ) ) . a r g ( H O M E P A G E ) ) ; 
 } 
   ^ # i f   d e f i n e d ( Q _ O S _ M A C ) 
 # i n c l u d e   < Q T i m e r > 
 # e n d i f 
  � # i n c l u d e   < Q F i l e > 
 # i n c l u d e   < Q D i r > 
 # i n c l u d e   < Q D a t a S t r e a m > 
 # i n c l u d e   < Q C l i p b o a r d > 
 # i n c l u d e   < Q F i l e D i a l o g > 
 # i n c l u d e   < Q F o n t D i a l o g > 
 # i n c l u d e   < Q I n p u t D i a l o g > 
 # i n c l u d e   < Q M e s s a g e B o x > 
 # i n c l u d e   < Q S e t t i n g s > 
 # i n c l u d e   < Q S c r o l l B a r > 
 # i n c l u d e   < Q F o n t > 
 # i n c l u d e   < Q F o n t M e t r i c s > 
  6         v o i d   O n F i n d I t e m ( ) ; 
         v o i d   O n F i n d N e x t I t e m ( ) ; 
         v o i d   O n E d i t I t e m ( ) ; 
         v o i d   O n C l i p b o a r d D a t a C h a n g e d ( ) ; 
         v o i d   O n C l i p b o a r d C h a n g e d ( Q C l i p b o a r d : : M o d e ) ; 
   � c o n f i g u r e   - s t a t i c   - d e b u g - a n d - r e l e a s e   - o p e n s o u r c e   - p l a t f o r m   w i n 3 2 - g + +   - q t - l i b j p e g   $ v o i d   O n L o a d D a t a ( ) ;   r c l a s s   C l i p h i s t W i n d o w   :   p u b l i c   Q M a i n W i n d o w 
 { 
         Q _ O B J E C T 
  � / * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
   * 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	   * 
   *   T h i s   f i l e   i s   p a r t   o f   t h e   C l i p h i s t 2   p a c k a g e   ( a   c l i p b o a r d   h i s t o r y   a p p l i c a t i o n )     * 
   * 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	   * 
   *   C o p y r i g h t   ( C )   2 0 0 9 - 2 0 1 0   b y   M i c h a e l   N e u r o t h . 	 	 	 	 	 	 	 	           * 
   *                                                                                                                                                               * 
   *   T h i s   p r o g r a m   i s   f r e e   s o f t w a r e ;   y o u   c a n   r e d i s t r i b u t e   i t   a n d / o r   m o d i f y 	 	 	   * 
   *   i t   u n d e r   t h e   t e r m s   o f   t h e   G N U   G e n e r a l   P u b l i c   L i c e n s e   a s   p u b l i s h e d   b y         	 	   * 
   *   t h e   F r e e   S o f t w a r e   F o u n d a t i o n ;   e i t h e r   v e r s i o n   2   o f   t h e   L i c e n s e ,   o r                           * 
   *   ( a t   y o u r   o p t i o n )   a n y   l a t e r   v e r s i o n .                                                                                       * 
   *                                                                                                                                                               * 
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * /    c l i p h i s t 2