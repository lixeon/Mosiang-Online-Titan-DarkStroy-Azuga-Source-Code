namespace web
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.HtmlControls;
    using System.Web.UI.WebControls;

    public class buy : Page
    {
        public string comment;
        protected DropDownList DropDownList1;
        public static int g;
        public float gold;
        public int goldoffer;
        public static int id;
        public int igold;
        public int iprice;
        public int isbb;
        public static int itemid;
        public string name;
        public static int p;
        public string pic;
        public float price;
        public int priceoffer;
        protected HtmlInputButton Submit1;
        public string vip;

        private void InitializeComponent()
        {
            this.Submit1.ServerClick += new EventHandler(this.Submit1_ServerClick);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            if (this.Session["useridx"] != null)
            {
                if (!this.Page.IsPostBack)
                {
                    this.DropDownList1.DataSource = new DataProviders().ExecuteSqlDs("select character_idx,character_name from mhgame..tb_character  where substring(character_name,1,1)!='@' and user_idx=" + this.Session["useridx"].ToString(), "DropDownList1");
                    this.DropDownList1.DataTextField = "character_name";
                    this.DropDownList1.DataValueField = "character_idx";
                    this.DropDownList1.DataBind();
                    for (int i = 0; i < this.DropDownList1.Items.Count; i++)
                    {
                        this.DropDownList1.Items[i].Text = new system().ConvertToBig5(this.DropDownList1.Items[i].Text.ToString(), 950);
                    }
                }
            }
            else
            {
                base.Response.Redirect("login.aspx?returnurl=/shop.aspx");
            }
            if (base.Request.QueryString["id"] != null)
            {
                DataProviders providers = new DataProviders();
                id = int.Parse(base.Request.QueryString["ID"]);
                SqlDataReader reader = providers.ExecuteSqlDataReader("select a.*,b.isbb from web_item a,web_itemtype b where a.type=b.id and a.id=" + id);
                if (reader.Read())
                {
                    this.name = reader["name"].ToString();
                    this.comment = reader["comment"].ToString();
                    itemid = int.Parse(reader["itemid"].ToString());
                    this.isbb = int.Parse(reader["isbb"].ToString());
                    this.pic = reader["pic"].ToString();
                    this.price = float.Parse(reader["price"].ToString());
                    this.gold = float.Parse(reader["gold"].ToString());
                    this.iprice = (int) this.price;
                    this.igold = (int) this.gold;
                    this.priceoffer = (int) (this.price * float.Parse(base.Application["game.vipoffer"].ToString()));
                    this.goldoffer = (int) (this.gold * float.Parse(base.Application["game.vipoffer"].ToString()));
                    p = (int) this.price;
                    g = (int) this.gold;
                    if (this.Session["weblevel"].ToString() == "1")
                    {
                        p = this.priceoffer;
                        g = this.goldoffer;
                        this.vip = "黃金會員";
                    }
                    if (this.Session["weblevel"].ToString() == "2")
                    {
                        p = this.priceoffer;
                        g = this.goldoffer;
                        this.vip = "白金會員";
                    }
                    if (this.Session["weblevel"].ToString() == "3")
                    {
                        p = this.priceoffer;
                        g = this.goldoffer;
                        this.vip = "鑽石會員";
                    }
                    else if (this.Session["weblevel"].ToString() == "0")
                    {
                        p = (int) this.price;
                        g = (int) this.gold;
                        this.vip = "普通會員";
                    }
                }
                else
                {
                    reader.Close();
                    providers.CloseConn();
                    base.Response.Redirect("shop.aspx");
                }
                reader.Close();
                providers.CloseConn();
            }
            else
            {
                base.Response.Redirect("shop.aspx");
            }
        }

        private void Submit1_ServerClick(object sender, EventArgs e)
        {
            if (this.DropDownList1.Items.Count == 0)
            {
                base.Response.Write("<script language=javascript>alert(\"請先創建遊戲角色再來購買唷\")</script>");
            }
            else
            {
                int chaidxs = int.Parse(this.DropDownList1.SelectedValue);
                int useridx = int.Parse(this.Session["useridx"].ToString());
                string str = new WebLogic().buyitem(this.Session["userid"].ToString(), useridx, chaidxs, itemid, p, g, this.isbb);
                base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
            }
        }
    }
}
