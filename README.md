

1.�����ObjectManager,��ObjectCreater ��

ObjectManager���ڵĹ���Object�б� �� Draw Objects

ObjectCreater ���ù���ģʽ���� Object


2.��� MeshFilter �洢 Object�� ���ݣ�vertices �� Indices��


3.ͨ��ImGui �������� Cube,Sphere(��δ���)��Щ����3D Object

��Component�����ŵ���ImGui,ʹ�ò����ɵ���

��Դ��ͨ��LightComponent�ı���ɫ ��С��



1. ObjectManager and ObjectCreater classes have been added

ObjectManager is used to manage the list of Objects and Draw Objects

ObjectCreater USES the factory pattern to generate objects


2. Add MeshFilter to store the data of objects (vertices and Indices)


3. Generate Cube,Sphere(not yet added) these basic 3D objects through ImGui control.

Put Component parameters in the ImGui to make the parameters tunable.

Light sources can change color sizes through LightComponent.