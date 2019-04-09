namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class itemlevelup : UserControl
    {
        protected Button btnlvup;
        protected DropDownList ddchalist;
        protected DropDownList dditem;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator Requiredfieldvalidator6;

        private void btnlvup_Click(object sender, EventArgs e)
        {
            int useridx = int.Parse(base.Session["useridx"].ToString());
            int chaidx = int.Parse(this.ddchalist.SelectedValue.ToString());
            int itemidx = int.Parse(this.dditem.SelectedValue.ToString());
            int needgold = int.Parse(base.Application["game.itemlvupgold"].ToString());
            Random random = new Random();
            int maxValue = int.Parse(base.Application["game.itemlvuprate"].ToString());
            int rate = random.Next(maxValue);
            string str = new WebLogic().itemlevelup(base.Session["userid"].ToString(), useridx, chaidx, itemidx, needgold, rate);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void ddchalist_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.ddchalist.SelectedValue);
            if (num == 0)
            {
                this.dditem.Items.Clear();
            }
            else
            {
                this.dditem.DataSource = new DataProviders().ExecuteSqlDs("select b.name,a.item_dbidx  from mhgame..tb_item a,mhcmember..Web_LvUpItemList b where a.item_idx = b.id and character_idx=" + num, "dditem");
                this.dditem.DataTextField = "name";
                this.dditem.DataValueField = "item_dbidx";
                this.dditem.DataBind();
            }
        }

        private void InitializeComponent()
        {
            this.ddchalist.SelectedIndexChanged += new EventHandler(this.ddchalist_SelectedIndexChanged);
            this.btnlvup.Click += new EventHandler(this.btnlvup_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            if (base.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
            else if (!this.Page.IsPostBack)
            {
                this.ddchalist.DataSource = new DataProviders().ExecuteSqlDs("select character_idx,character_name from mhgame..tb_character  where substring(character_name,1,1)!='@' and user_idx=" + base.Session["useridx"].ToString(), "ddchalist");
                this.ddchalist.DataTextField = "character_name";
                this.ddchalist.DataValueField = "character_idx";
                this.ddchalist.DataBind();
                for (int i = 0; i < this.ddchalist.Items.Count; i++)
                {
                    this.ddchalist.Items[i].Text = new system().ConvertToBig5(this.ddchalist.Items[i].Text.ToString(), 950);
                }
                this.ddchalist.Items.Insert(0, new ListItem("請選擇角色", "0"));
            }
        }
    }
}
