from nltk.tree import *

# assign your output (generalied list of the syntax tree) to varaible text
# text = "[<P>[<S> [<A> a = 10 ;]][<S> [<for>]][<S> [<W> write SPACE a]][<S> [<W> write SPACE b]]]"
# text = "[<P> [<S> [<D> int SPACE a , b , i ;] ] [<P> [<S> [<A> a = 10 ;]] [<P> [] ] ] ]"
text = "[a b [c d]]"

text = text.replace("(", "ob")  # in the syntax tree, 'ob' will display in place of '('
text = text.replace(")", "cb")  # in the syntax tree, 'cb' will display in place of ')'
text = text.replace("[", "(")
text = text.replace("]", ")")


tree = Tree.fromstring(text)
tree.pretty_print(unicodelines=True, nodedist=2)
