Non-Metric Space Library
=================


Leo(nid) Boytsov is a maintainer.  


General information
-----------------------

A detailed description is given [in the manual](docs/manual.pdf). Note that the directory **similarity_search** contains an Eclipse project that can be imported into [The Eclipse IDE for C/C++ Developers](http://www.eclipse.org/downloads/moreinfo/c.php). A basic description of how to import
the project into and debug it from Eclipse is given in the manual.

Most of this code is released under the
Apache License Version 2.0 http://www.apache.org/licenses/.

The LSHKIT, which is embedded in our library, is distributed under the GNU General Public License, see http://www.gnu.org/licenses/. 

Related papers:

* Boytsov, L., Bilegsaikhan. N., 2013. [Engineering Efficient and Effective Non-Metric Space Library.](http://boytsov.info/pubs/sisap2013.pdf)   In Proceedings of the 6th International Conference on Similarity Search and Applications (SISAP 2013). [**[BibTex]**](http://dblp.uni-trier.de/rec/bibtex/conf/sisap/BoytsovN13)  
* Boytsov, L., Bilegsaikhan. N., 2013. [Learning to Prune in Metric and Non-Metric Spaces.](http://boytsov.info/pubs/nips2013.pdf)   In Advances in Neural Information Processing Systems 2013. [**[BibTex]**](http://dblp.uni-trier.de/rec/bibtex/conf/nips/BoytsovN13)      



To acknowledge the use of the library, you could provide a link to this repository and/or cite our SISAP paper [**[BibTex]**](http://dblp.uni-trier.de/rec/bibtex/conf/sisap/BoytsovN13),   


Prerequisites
-----------------------

1. G++ 4.7 or the Intel compiler version 14.
2. **64-bit** Linux is recommended
3. cmake (GNU make is also required)
5. Boost (dev version)
6. GNU scientific library (dev version)
7. An Intel or AMD processor that supports SSE 4.2 is recommended


Quick start
-----------------------

To compile, go to the directory **similarity_search** and type:  
```bash
cmake .
make  
```


A more detailed description is given in [in the manual](docs/manual.pdf).  

Examples of using the software can be found in the directory sample_scripts. A good starting point is a script [sample_scripts/sample_run.sh](sample_scripts/sample_run.sh). This script uses small data sets stored in this repository. The complete data set can be obtained using the script [data/get_all_data.sh](data/get_all_data.sh). Beware: it is more than 5 GBs compressed! The Wikipedia datasets (sparse and dense vectors) is the largest part occupying 5 GB and 3GB, respectively. The Cayton collection is about 500 MB.

The downloaded data needs to be decompressed (use 7z  and gzip). Then, copy data files to a directory of choice and set the environment variable:  

```bash
export DATA_DIR=[path to the chosen directory with data files]
```


Sample scripts to tune the decision function for the VP-tree are in the directory [sample_scripts/nips2013/tunning](sample_scripts/nips2013/tunning).   
In addition, the directory [sample_scripts](sample_scripts) contains the full set of scripts that can be used to re-produce our NIPS'13 and SISAP'13 results.  Note that we also provide software to generate plots (which requires Python, Latex, and PGF).   


Data sets
-----------------------

We use several data sets, which were created either by other folks,
or using 3d party software. If you use these data sets, please, consider
giving proper credit.

The data set created by Lawrence Cayton (lcayton.com).
To download use [data/download_cayton.sh](data/download_cayton.sh).  
To acknowledge the use in an academic publication:

```
@inproceedings{cayton:2008,  
    title={Fast nearest neighbor retrieval for bregman divergences},  
    author={Cayton, Lawrence},   
    booktitle={Proceedings of the 25th international conference on Machine learning},  
    pages={112--119},   
    year={2008},   
    organization={ACM}  
}
```

The Wikipedia data sets were created using the [gensim library](https://github.com/piskvorky/gensim/). To download use the the script [data/download_wikipedia_sparse.sh](data/download_wikipedia_sparse.sh) and [data/download_wikipedia_lsi128.sh](data/download_wikipedia_lsi128.sh).   
To acknowledge the use in an academic publication:

```
@inproceedings{rehurek_lrec,  
    title = {{Software Framework for Topic Modelling   
            with Large Corpora}},  
    author = {Radim {\v R}eh{\r u}{\v r}ek and Petr Sojka},  
    booktitle = {{Proceedings of the LREC 2010 Workshop on New  
                Challenges for NLP Frameworks}},  
    pages = {45--50},  
    year = 2010,  
    month = May,  
    day = 22,  
    publisher = {ELRA},  
    address = {Valletta, Malta},  
    note={\url{http://is.muni.cz/publication/884893/en}},  
    language={English}  
} 
```

The Colors data set belongs to the Metric Spaces Library.
However, our software uses data in a different format.
Thus, we changed the format a bit (removed the header),
To download use [data/download_colors.sh](data/download_colors.sh).   
To acknowledge the use in an academic publication:

```
@misc{LibMetricSpace, 
    Author = {K.~Figueroa and G.{}~Navarro and E.~Ch\'avez},  
    Keywords = {Metric Spaces, similarity searching},  
    Note = {Available at {\tt http://www.sisap.org/Metric\_Space\_Library.html}},  
    Title = {Metric Spaces Library},  
    Year = {2007}  
} 
```




