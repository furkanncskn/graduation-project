# İvme Ölçer İle Hız Hesabı
<h6>Not: Proje tamamlanmıştır !!!</h6>

> **`Gürültünün ve istenmeyen sinyallerin filtrelenmesi`**
> 

[Test görsellerinde](#test-görselleri) 3 farklı değer görülmektedir. Bunlar;

1. **İvmeölçer sensör çıkışı**
2. **FIR filtresi çıkışı**
3. **IIR filtresi çıkışı**

İvmeölçer çıkışından elde edilen değeri direkt olarak uygulamamızda kullanamayız. Bunun sebebi, gürültüye ve üzerinde oluşabilecek kuvvetlere karşı hassas olmasıdır. İvmeölçeri hiçbir filtreden geçirmeden direkt olarak alıp bir ekranda verileri gösterdiğimizi düşünelim. İvmeölçeri elimize alıp, elimizi önümüzde durun masaya ve herhangi bir yere hafif bir şekilde vurduğumuzda çıkıştaki değişim aşağıda yer alan [1.test görselinde](#test-görselleri) ki kırmızı renkli olan çizgideki gibi veya daha kötüsü olabilir. Bu gibi anlık değişimlerin sistemimize olan etkisini minimuma indirebilmek için çeşitli filtreler kullanılır. Benim tercihim ise **<u>Moving Average</u>** filtresi oldu. Denklem yapısı ise şu şekildedir;

<p align="center">
<img width="250" height="90" src="https://user-images.githubusercontent.com/75426545/148693677-5623d430-a40f-4e25-9315-3f36b240ff7c.png" alt="Moving Average Filter">
</p>

Tabi bununla işlemimiz sonlamıyor, sisteme etki eden gürültüyü de temizleyebilmek için farklı bir filtre daha kullanıldı. Burada ki tercihim ise <u>**1.Derece IIR filtresi**</u> kullanmak oldu. Her eklenen filtre sistemin fazına etkide bulunur. IIR filtresinin sistemimize maliyeti yaklaşık 150 milisaniyelik bir gecikme olmuştur. Denklem yapısı ise şu şekildedir;

<p align="center">
<img width="250" height="90" src="https://user-images.githubusercontent.com/75426545/148693861-364641db-2540-4a90-9c01-9e3502e9eb31.gif" alt="Moving Average Filter">
</p>
