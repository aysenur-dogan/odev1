MPI Parallel Matrix Multiplication

Bu projede MPI (Message Passing Interface) kullanılarak paralel matris-matris çarpımı gerçekleştirilmiştir. Çalışma hem Python (mpi4py) hem de C (OpenMPI) dilleri kullanılarak geliştirilmiştir.

Kullanılan Teknolojiler

* Python
* C
* OpenMPI
* mpi4py
* VS Code
* GitHub

Proje İçeriği

Projede:

* Matris verileri a.txt ve b.txt dosyalarından okunmuştur.
* MPI kullanılarak matrisler paralel şekilde işlenmiştir.
* Scatter, Broadcast ve Gather işlemleri kullanılmıştır.
* Çalışma süreleri farklı çekirdek sayılarında ölçülmüştür.
* Speedup ve Efficiency hesaplamaları yapılmıştır.

Klasör Yapısı

data/       -> Matris dosyaları

python/     -> Python MPI kodları

c/          -> C MPI kodları

results.txt -> Performans sonuçları

Sonuç

Paralel programlama sayesinde işlem sürelerinde önemli ölçüde hızlanma elde edilmiştir. C dili Python’a göre daha yüksek performans göstermiştir.

Hazırlayanlar

* Ayşenur Doğan
* Sultan Demircan