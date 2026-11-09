import collections, statistics as st

def f(x):
    s = f"{int(round(x)):,}".replace(",", "\\,")
    return s

def ms(x):
    if x < 0:
        return "--"
    s = f"{x:.4f}" if x < 0.01 else f"{x:.3f}" if x < 10 else f"{x:.1f}" if x < 1000 else f"{x:.0f}"
    return s.replace(".", ",")

def passos(x):
    return "--" if x < 0 else f(x)

linhas = [l.rstrip("\n").split("\t") for l in open("dados/pior.tsv")]
out = []
t = ["\\newcommand{\\tabelapiorcaso}{",
     "\\setlength{\\tabcolsep}{3.5pt}\\footnotesize\\begin{tabular}{r rr rr rr rr rr}\\toprule & \\multicolumn{2}{c}{Força bruta} & \\multicolumn{2}{c}{Backtracking} & \\multicolumn{2}{c}{Held-Karp} & \\multicolumn{2}{c}{Guloso} & \\multicolumn{2}{c}{Recozimento}\\\\ $n$ & passos & ms & passos & ms & passos & ms & passos & ms & passos & ms\\\\ \\midrule"]
for r in linhas:
    n = int(r[1]); v = list(map(float, r[2:]))
    t.append(f"{n} & {passos(v[0])} & {ms(v[1])} & {passos(v[2])} & {ms(v[3])} & {passos(v[4])} & {ms(v[5])} & {passos(v[6])} & {ms(v[7])} & {passos(v[8])} & {ms(v[9])}\\\\")
t.append("\\bottomrule\\end{tabular}}")
out.append("\n".join(t))

def curva(i):
    return " ".join(f"({r[1]},{r[i]})" for r in linhas if float(r[i]) >= 0)

for nome, ylabel, off in (("graficotempo", "Tempo (ms)", 1), ("graficonos", "Passos", 0)):
    g = [f"\\newcommand{{\\{nome}}}{{",
         f"\\begin{{tikzpicture}}\\begin{{semilogyaxis}}[width=0.9\\linewidth,height=6.5cm,xlabel={{$n$}},ylabel={{{ylabel}}},legend style={{at={{(0.5,1.04)}},anchor=south,font=\\small}},legend columns=3,grid=both,xtick={{6,8,10,12,14,16,18,20}}]"]
    for k, (cor, nm) in enumerate((("red", "Força bruta"), ("blue", "Backtracking"), ("black", "Held-Karp"), ("green!60!black", "Guloso"), ("orange", "Recozimento"))):
        g.append(f"\\addplot[color={cor},mark=*] coordinates {{{curva(2 + 2 * k + off)}}};\\addlegendentry{{{nm}}}")
    g.append("\\end{semilogyaxis}\\end{tikzpicture}}")
    out.append("\n".join(g))

R = collections.defaultdict(list)
for l in open("dados/varre.tsv"):
    c = l.split("\t")
    R[(int(c[1]), float(c[2]))].append(list(map(float, c[3:])))
# colunas: 0 semente,1 ciclo,2 nos_bt,3 estados_hk,4 hk_ok,5 gi,6 gw,7 rec(de 5)

def med(L, i):
    return st.median([x[i] for x in L]) if L else None

def cel(v):
    return "--" if v is None else f(v)

def pct(k):
    r = R[k]
    return 100 * sum(x[1] for x in r) / len(r)

t = ["\\newcommand{\\tabeladensidade}{",
     "\\begin{tabular}{r r r r r r}\\toprule $n$ & $p$ & com ciclo & \\multicolumn{2}{c}{Backtracking (nós, mediana)} & Held-Karp\\\\ & & (\\%) & com ciclo & sem ciclo & (estados)\\\\ \\midrule"]
for n, ps in ((14, [0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.50]), (18, [0.10, 0.15, 0.20, 0.25, 0.35])):
    for p in ps:
        r = R[(n, p)]
        sim = [x for x in r if x[1]]; nao = [x for x in r if not x[1]]
        t.append(f"{n} & {str(p).replace('.', ',')} & {pct((n, p)):.0f} & {cel(med(sim, 2))} & {cel(med(nao, 2))} & {cel(med(r, 3))}\\\\")
    if n == 14:
        t.append("\\midrule")
t.append("\\bottomrule\\end{tabular}}")
out.append("\n".join(t))

g = ["\\newcommand{\\graficodensidade}{",
     "\\begin{tikzpicture}\\begin{axis}[width=0.9\\linewidth,height=6cm,xlabel={Densidade $p$},ylabel={Digrafos com ciclo (\\%)},legend pos=south east,grid=both]"]
for cor, n, ps in (("blue", 14, [0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.35, 0.40, 0.50]), ("red", 18, [0.10, 0.15, 0.20, 0.25, 0.35])):
    g.append(f"\\addplot[color={cor},mark=*] coordinates {{{' '.join(f'({p},{pct((n, p)):.1f})' for p in ps)}}};\\addlegendentry{{$n={n}$}}")
g.append("\\end{axis}\\end{tikzpicture}}")
out.append("\n".join(g))

t = ["\\newcommand{\\tabelan}{",
     "\\begin{tabular}{r r r r r r}\\toprule $n$ & com ciclo & \\multicolumn{3}{c}{Backtracking (nós)} & Held-Karp\\\\ & (\\%) & mediana com ciclo & mediana sem ciclo & maior visto & (estados)\\\\ \\midrule"]
for n in (10, 12, 14, 16, 18, 20):
    r = R[(n, 0.35)]
    sim = [x for x in r if x[1]]; nao = [x for x in r if not x[1]]
    t.append(f"{n} & {pct((n, 0.35)):.0f} & {cel(med(sim, 2))} & {cel(med(nao, 2))} & {f(max(x[2] for x in r))} & {cel(med(r, 3))}\\\\")
t.append("\\bottomrule\\end{tabular}}")
out.append("\n".join(t))

t = ["\\newcommand{\\tabelaheuristicas}{",
     "\\begin{tabular}{r r r r r r r}\\toprule $n$ & $p$ & com ciclo & guloso & Warnsdorff & \\multicolumn{2}{c}{recozimento}\\\\ & & (digrafos) & achou & achou & execuções & digrafos\\\\ \\midrule"]
for n, ps in ((14, [0.25, 0.35]), (18, [0.25, 0.35]), (20, [0.35])):
    for p in ps:
        sim = [x for x in R[(n, p)] if x[1]]
        t.append(f"{n} & {str(p).replace('.', ',')} & {len(sim)} & {int(sum(x[5] for x in sim))} & {int(sum(x[6] for x in sim))} & {int(sum(x[7] for x in sim))}/{5 * len(sim)} & {sum(x[7] > 0 for x in sim)}/{len(sim)}\\\\")
    if n != 20:
        t.append("\\midrule")
t.append("\\bottomrule\\end{tabular}}")
out.append("\n".join(t))

G = collections.defaultdict(lambda: [0, 0, 0, 0])
for l in open("dados/grandes.tsv"):
    c = l.split("\t"); k = (int(c[1]), float(c[2]))
    G[k][0] += 1
    for i in range(3):
        G[k][i + 1] += int(c[4 + i])
t = ["\\newcommand{\\tabelagrandes}{",
     "\\begin{tabular}{r r r r r}\\toprule $n$ & $p$ & guloso & Warnsdorff & recozimento\\\\ \\midrule"]
for p in (0.35,):
    for n in (30, 50, 100):
        k = (n, p)
        t.append(f"{n} & {str(p).replace('.', ',')} & {G[k][1]}/{G[k][0]} & {G[k][2]}/{G[k][0]} & {G[k][3]}/{G[k][0]}\\\\")
    if p == 0.10:
        t.append("\\midrule")
t.append("\\bottomrule\\end{tabular}}")
out.append("\n".join(t))

t = ["\\newcommand{\\tabelaciclosimples}{",
     "\\begin{tabular}{r r r}\\toprule $n$ & recozimento (\\% de 100 execuções) & guloso\\\\ \\midrule"]
for l in open("dados/ciclo.tsv"):
    c = l.split("\t")
    t.append(f"{c[1]} & {c[2]} & {'achou' if int(c[3]) else 'falhou'}\\\\")
t.append("\\bottomrule\\end{tabular}}")
out.append("\n".join(t))

open("dados.tex", "w", encoding="utf-8").write("\n".join(out) + "\n")
