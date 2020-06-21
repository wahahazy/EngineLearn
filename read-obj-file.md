# 读取obj文件

## 具体目标格式-polygonal geometry
face(f)</br>
vertex data: position(v)、normal(vn)、texture-uv(vt)</br>
usemtl material_name:用于指定后续顶点所使用的材质名称。
Smoothing group(s): 不清楚设置具体的数值的意义<br> 


for example: </br>
usemtl fox_material</br>
s 1</br>
f v/vt/vn  v/vt/vn  v/vt/vn </br>
其中：f表示三角形，后面紧跟着三个顶点的数据。
</br>

note:</br>
1.右手坐标系。若三角形顶点是逆时针的话，则表示三角面是对着观察者的。</br>
2.vt、vn可以缺失， 但必须所有顶点都保持一致的格式。</br>
3.对象的所有组成三角形是正面还是背面，是统一的。即：一致保持朝向对象内部或者外部。</br>
4.材质一旦分配了，它不能被关闭。只能被改变。</br>
5.若材质名称缺失，则使用白色。</br>
6.#开头的表示注释行</br>

## 材质格式
newmtl fox_material 材质名称</br>
Ns 反射指数 定义了反射高光度。该值越高则高光越密集，一般取值范围在0~1000</br>
Ka 材质的环境光（ambient color）</br>
Kd 散射光（diffuse color）</br>
Ks 镜面光（specular color）</br>
Ni 折射值 可在0.001到10之间进行取值。若取值为1.0，光在通过物体的时候不发生弯曲。</br>
d 透明度，取值为1.0表示完全不透明，取值为0.0时表示完全透明</br>
illum 2 指定材质的光照模型。illum后面可接0~10范围内的数字参数。2为highlight on</br>
map_Kd texture.png 为漫反射指定颜色纹理文件</br>

## 步骤
1.读取v、vt、vn的数据到数组中。若这三组的数量不一致（0除外），则返回"读取失败"。
2.根据第一步的数据，创建geometry。
3.读取.mtl数据，创建material。

跟以前不一样的是多个material。
