#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

struct no {
	int chave;
	char cor;
	struct no *pai;
	struct no *esq;
	struct no *dir;
};
typedef struct no No;

No * externo;

// ----------- AUXILIARES ----------- //

void desalocar_rn(No ** pt) {
	
	if ((*pt) != externo) {
		
			
		desalocar_rn(&(*pt)->esq);
		desalocar_rn(&(*pt)->dir);
		
		free((*pt));
		
	}
	
}

No * busca_binaria(int chave, No **ptraiz) {
	No * x;
	x = (*ptraiz);

	while (x != externo && x->chave != chave) {
		
		if (chave < x->chave) {
			x = x->esq;
		} else {
			x = x->dir;
		}
	}

	return x;
}

void pre_ordem(No * pt) {

	printf("%d-%c  ", pt->chave, pt->cor);
	
	if	(pt->esq != externo) 
		pre_ordem(pt->esq);
	if	(pt->dir != externo) 
		pre_ordem(pt->dir);
}

No * inicializa_no(int val) {

	No * novo = malloc(sizeof(No));
	novo->esq = novo->dir = NULL;
	novo->chave = val;
	novo->cor = ' ';

	return novo;

}

// ----------- INSERIR NA RN ----------- //

void rotacao_esq_rn(No * x, No ** ptraiz) {

	No * y = (x)->dir;
	(x)->dir = y->esq;

	if (y->esq != externo) {
		y->esq->pai = (x);
	}

	y->pai = (x)->pai;

	if (x->pai == externo) {
		(*ptraiz) = y;

	} else if ((x) == (x)->pai->esq) {
		(x)->pai->esq = y;

	} else {
		(x)->pai->dir = y;

	}

	y->esq = (x);
	(x)->pai = y;
}

void rotacao_dir_rn(No * x, No ** ptraiz) {
	No * y = (x)->esq;
	(x)->esq = y->dir;

	if (y->dir != externo) {
		y->dir->pai = (x);
	}

	y->pai = (x)->pai;
	
	if (x->pai == externo) {
		(*ptraiz) = y;

	} else if ((x) == (x)->pai->dir) {
		(x)->pai->dir = y;

	} else {
		(x)->pai->esq = y;

	}

	y->dir = (x);
	(x)->pai = y;
}

void rota_rn(No * z, No ** ptraiz) {
	
	while ((z)->pai->cor == 'R') {

		if ((z)->pai == (z)->pai->pai->esq) {

			No * y = (z)->pai->pai->dir;

			if (y->cor == 'R') {

				(z)->pai->cor = y->cor = 'N';

				(z)->pai->pai->cor = 'R';

				(z) = (z)->pai->pai;

			} else {

				if ( (z) == (z)->pai->dir ) {

					(z) = (z)->pai;
					rotacao_esq_rn((z), ptraiz);
				}

				(z)->pai->cor = 'N';
				(z)->pai->pai->cor = 'R';
				rotacao_dir_rn((z)->pai->pai, ptraiz);

			}

		} else {

			No * y = (z)->pai->pai->esq;

			if (y->cor == 'R') {

				(z)->pai->cor = y->cor = 'N';

				(z)->pai->pai->cor = 'R';

				(z) = (z)->pai->pai;

			} else {

				if ( (z) == (z)->pai->esq ) {

					(z) = (z)->pai;
					rotacao_dir_rn(z, ptraiz);
				}

				(z)->pai->cor = 'N';
				(z)->pai->pai->cor = 'R';
				rotacao_esq_rn((z)->pai->pai, ptraiz);
			}

		}

	}

	(*ptraiz)->cor = 'N';

}

void inserir_rn(No ** z, No ** ptraiz) {

	No * y = externo;
	No * x = (*ptraiz);

	while (x != externo) {
		y = x;

		if ((*z)->chave == x->chave) {
			
			printf("Chave existente\n");
			y = NULL;
			x = externo;

		} else {

			if ((*z)->chave < x->chave) {

				x = x->esq;

			} else {

				x = x->dir;
			}

		}

	}


	if ( y != NULL) {
		(*z)->pai = y;

		if (y == externo) {
			(*ptraiz) = (*z);

		} else {

			if ((*z)->chave < y->chave) {
				y->esq = (*z);
			} else {
				y->dir = (*z);
			}

		}

		(*z)->esq = externo;
		(*z)->dir = externo;
		(*z)->cor = 'R';

		rota_rn((*z), ptraiz);
	}
}

// ----------- REMOVER NA RN ----------- //

No * sucessor(No * z) {

	No * s = z->dir;
						
	if (s->esq != externo) {
		No * PaiS = s;
		while (s->esq != externo) {
			PaiS = s;
			s = s->esq;
		}
	}

	return s;
}

void mover_pai_rn(No *u, No * v, No ** ptraiz) {
	
	if (u->pai == externo) {
		(*ptraiz) = v;
	
	} else {
		if (u == u->pai->esq) {
			u->pai->esq = v;
	
		} else {
			u->pai->dir = v;
	
		}
	}
	
	v->pai = u->pai;
}

void rota_remover_rn(No * x, No ** ptraiz) {
	No * w; //irmão de x
	
	while (x != (*ptraiz) && x->cor != 'R') {
	
		if (x->pai->esq == x) {
			w = x->pai->dir; //irmão direito
			
			// caso 1
			if (w->cor == 'R') {
				w->cor = 'N';
				x->pai->cor = 'R';
				rotacao_esq_rn(x->pai, ptraiz);
				w = x->pai->dir;
			}

			// caso 2
			if (w->dir->cor == 'N' && w->esq->cor == 'N') {
				w->cor = 'R';
				x = x->pai;

			} else {
				
				//caso 3
				if (w->dir->cor == 'N') {
					w->esq->cor = 'N';
					w->cor = 'R';
					rotacao_dir_rn(w, ptraiz);
					w = x->pai->dir;
				}
				
				//caso 4
				w->cor = x->pai->cor;
				x->pai->cor = 'N';
				w->dir->cor = 'N';
				rotacao_esq_rn(x->pai, ptraiz);
				x = (*ptraiz);
			}

		} else {
			w = x->pai->esq; //irmão esquerdo

			//caso 1
			if (w->cor == 'R') {
				w->cor = 'N';
				x->pai->cor = 'R';
				rotacao_dir_rn(x->pai, ptraiz);
				w = x->pai->esq;
			}

			//caso 2
			if (w->dir->cor == 'N' && w->esq->cor == 'N') {
				w->cor = 'R';
				x = x->pai;

			} else {
	
				//caso 3
				if (w->esq->cor == 'N') {
					w->dir->cor = 'N';
					w->cor = 'R';
					rotacao_esq_rn(w, ptraiz);
					w = x->pai->esq;
				}
				
				//caso 4
				w->cor = x->pai->cor;
				x->pai->cor = 'N';
				w->esq->cor = 'N';
				rotacao_dir_rn(x->pai, ptraiz);
				x = (*ptraiz);
			}

		}

	}

	x->cor = 'N';
}

void remover_rn(No *z, No ** ptraiz){
	No * x;
	No * y = z;
	char cor_original = y->cor;
	
	if (y->esq == externo) {
		x = z->dir;
		mover_pai_rn(z, z->dir, ptraiz);

	} else {
		if (y->dir == externo) {
			x = z->esq;
			mover_pai_rn(z, z->esq, ptraiz);

		} else {
			y = sucessor(z);
			cor_original = y->cor;
			x = y->dir;

			if (y->pai == z) {
				x->pai = y; 
				
			} else {
				mover_pai_rn(y, y->dir, ptraiz);
				y->dir = z->dir;
				y->dir->pai = y;
			}
			
			mover_pai_rn(z, y, ptraiz);
			y->esq = z->esq;
			y->esq->pai = y;
			y->cor = z->cor;
		}
	}

	if (cor_original == 'N') {
		rota_remover_rn(x, ptraiz);
	}
}

// ----------- QTD DE NÓS NA RN ----------- //

int contar_nos_rn(No * pt) {
	int count = 1;
	int c_dir = 0;
	int c_esq = 0;

	// Desconsidera os nós externos
	if (pt == externo) {
		return 0;
	}

	c_esq = contar_nos_rn(pt->esq);
	c_dir = contar_nos_rn(pt->dir);
	
	return count+c_esq+c_dir;
}

// ----------- VERIFICAR RN E CALCULAR ALTURA NEGRA  ----------- //

int verifica_rn(No * pt, int * b_height) {
	
	int b_height_esq = 0;
	int b_height_dir = 0;
	int verif_dir = 0;
	int verif_esq = 0;

	if (pt == externo) {
		*b_height = 0;
		return 1;
	}

	verif_esq = verifica_rn(pt->esq, &b_height_esq);
	verif_dir = verifica_rn(pt->dir, &b_height_dir);
	
	b_height_esq = pt->esq->cor == 'N' ? b_height_esq + 1 : b_height_esq;
	b_height_dir = pt->dir->cor == 'N' ? b_height_dir + 1 : b_height_dir;

	*b_height = b_height_esq > b_height_dir ? b_height_esq : b_height_dir;

	if (b_height_esq != b_height_dir) {
		return 0;
	}

	return verif_dir && verif_esq;
}


// ----------- TESTES ----------- //

void teste_rn(int num_rn, int num_ins, int num_rem) {
	//num_rn = Número de árvores RN
	//num_ins = Número de inserções 
	//num_rem = Número de remoções
	
	No * ptraiz = externo;
	
	int i, j, indice, aux, verifica, chave;
	
	int b_height = 0;
	int count = 0;
	
	No * no = NULL;
	
	int * chaves = malloc(sizeof(int) * num_ins);
	
	for (i = 0; i < num_rn; i++) {
		
		printf("\n\n----- Árvore nº %d: -----\n\n", i+1);
		
		//Inserções
		printf("\nInserções:\n");
		
		for (j = 0; j < num_ins; j++) {
			
			chave = rand() % 100001;			
			
			// Verifica se a chave já existe na árvore 
			// para evitar a tentativa de inserir chaves repetidas
			no = busca_binaria(chave, &ptraiz);
			
			if (no == externo) {
				
				no = inicializa_no(chave);
				
				inserir_rn(&no, &ptraiz);
				
				// Armazena as chaves inseridas para poder usá-las na remoção
				chaves[j] = chave;
				
				no = NULL;
				
			} else {
				
				j--;	
				
			}
			
		}
		
		// Verificações
		
		count = contar_nos_rn(ptraiz);
		printf("Nº de nós: %d\n", count);
		verifica = verifica_rn(ptraiz, &b_height);
		printf("Altura negra: %d\n", b_height);
		printf("É rubro-negra? %s\n", verifica == 1 ? "sim" : "não");
		
		if (verifica != 1 && count != num_ins) {
			break;
		}

		b_height = count = 0;
		
		// Remoções
		printf("\nRemoções:\n");
		
		for (j = 0; j < num_rem; j++) {
			
			// Busca uma chave aleatória dentre as inseridas para remover
			indice = rand() % (num_ins - j);			
			no = busca_binaria(chaves[indice], &ptraiz);
			remover_rn(no, &ptraiz);
			
			//Move o valor removido para o final do array para que não seja selecionado na próxima iteração			
			aux = chaves[indice];
			chaves[indice] = chaves[num_ins - (j+1)];
			chaves[num_ins - (j+1)] = aux;
		}
		
	
		//Verificações		
		count = contar_nos_rn(ptraiz);
		printf("Nº de nós: %d\n", count);
		verifica = verifica_rn(ptraiz, &b_height);
		printf("Altura negra: %d\n", b_height);
		printf("É rubro-negra? %s", verifica == 1 ? "sim" : "não");
		
		if (verifica != 1 && count != (num_ins - num_rem)) {
			break;
		}

		b_height = count = 0;
		
		desalocar_rn(&ptraiz);
		
		ptraiz = externo;
	
	}	
	
}

int main(void) {
	setlocale(LC_ALL, "Portuguese");
	srand( time(NULL) );

	externo = malloc(sizeof(No));
	externo->cor = 'N';

	teste_rn(1000, 10000, 1000);
	printf("\n\n\n\n");

  	return 0;
}

// Aluna: Natália Lídia Lima Coelho - 472327
